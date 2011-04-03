// Boost
#include <boost/asio.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/random.hpp>

// OpenAL
#include <AL/al.h>
#include <AL/alc.h>

// OpenCV
#include <opencv2/opencv.hpp>

#include "list.h"
#include "LSD.h"
#include "VideoEncoder.h"

using namespace std;
using boost::asio::ip::tcp;
using namespace boost::posix_time;
using boost::posix_time::ptime;
using boost::posix_time::time_duration;

int audio_capture_device;
int audio_sample_rate;
std::string container;
bool flag_disable_audio;
bool flag_disable_video;
bool flag_generate_audio;
bool flag_generate_video;
bool flag_lsd;
std::string server_url;
int stream_bitrate;
std::string username;
int video_capture_device;
int video_width;
int video_height;
int video_frame_rate;

// Boost
boost::mt19937 rng;
boost::uniform_int<> six(-128, 127);
boost::variate_generator<boost::mt19937&, boost::uniform_int<> >die(rng, six);

// FFmpeg
VideoEncoder encoder;
AVFrame* frame;
AVFrame* readyFrame;
int nSampleSize;
char* sample;
URLContext* StreamToUrl;

// OpenAL
ALCdevice* dev[2];
ALCcontext* ctx;
ALuint source, buffers[3];
ALchar* Buffer;
ALuint buf;
ALint val;
ALint iSamplesAvailable;
int nBlockAlign;
ALCdevice* pDevice;
ALCcontext* pContext;
ALCdevice* pCaptureDevice;
const ALCchar* szDefaultCaptureDevice;

// OpenCV
CvCapture* capture;
IplImage* CVframe;
IplImage* CVframeWithText;
IplImage* destination;
IplImage* redchannel;
IplImage* greenchannel;
IplImage* bluechannel;
CvFont font;
CvPoint UL;
CvPoint LR;

int64_t desiredTimeForCaptureFame;
int64_t spendedTimeForCaptureFame;

int64_t desiredTimeForMain;
int64_t spendedTimeForMain;

boost::timer timerForCaptureFame;
boost::timer timerForMain;

int get_evan(int number)
{
	while (number % 4 != 0)
		++number;
	return number;
}

void replace_or_merge(std::string &a, const std::string &b, const std::string &c)
{
	const std::string::size_type pos_b_in_a = a.find(b);
	if (pos_b_in_a == std::string::npos)
		a.insert(0, c);
	else
		a.replace(pos_b_in_a, b.length(), c);
}

void get_server()
{
	cout << "Please input stream URL (ex. http://127.0.0.1:4773/ )\n";
	cin >> server_url;
	replace_or_merge(server_url, "http://", "tcp://");
}

void get_name()
{
	cout << "Please your name (ex. georg )\n";
	cin >> username;
}

void init_opencv()
{
	if (flag_disable_video)
	{
		encoder.hasVideo = false;
		return;
	}

	if (flag_generate_video)
	{
		encoder.hasVideo = true;
		cvInitFont(&font, CV_FONT_HERSHEY_DUPLEX, 2, 1, 0.0, 3, CV_AA);
		CvPoint UL = { 0, 0 };
		CvPoint LR = { video_width, video_height };
		CVframe = cvCreateImage(cvSize(video_width, video_height), 8, 4);
		CVframeWithText = cvCreateImage(cvSize(video_width, video_height), 8, 4);
		cvRectangle(CVframe, UL, LR, CV_RGB(0, 254, 53), CV_FILLED);
		cvPutText(CVframe, username.c_str(), cvPoint(0, video_height - 10), &font, CV_RGB(1, 1, 1));
	}
	else
	{
		CamerasListNamespace::CamerasList* CamList = new CamerasListNamespace::CamerasList();
		video_capture_device = CamList->SelectFromList();

		if (video_capture_device == 999)
			encoder.hasVideo = false;
		else
		{
			capture = cvCaptureFromCAM(video_capture_device);
			cvSetCaptureProperty(capture, CV_CAP_PROP_FPS, video_frame_rate);
			cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH, (double)video_width);
			cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT, (double)video_height);
		}

		if (!capture)
		{
			encoder.hasVideo = false;
			fprintf(stderr, "Cannot initialize selected webcam!\n");
			cout << endl;
			return;
		}

		CVframe = cvQueryFrame(capture);
		destination = cvCreateImage(cvSize(video_width, video_height), CVframe->depth, CVframe->nChannels);
		redchannel = cvCreateImage(cvGetSize(destination), 8, 1);
		greenchannel = cvCreateImage(cvGetSize(destination), 8, 1);
		bluechannel = cvCreateImage(cvGetSize(destination), 8, 1);
		encoder.hasVideo = true;
	}
}

void init_openal(int fps)
{
	if (flag_disable_audio)
	{
		encoder.hasAudio = false;
		return;
	}

	if (flag_generate_audio)
	{
		encoder.hasAudio = true;
		nSampleSize = (int)(2.0f * audio_sample_rate / fps);
		nBlockAlign = 1 * 16 / 8;
		Buffer = new ALchar[nSampleSize];
	}
	else
	{
		nSampleSize = (int)(2.0f * audio_sample_rate / fps);
		nBlockAlign = 1 * 16 / 8;
		Buffer = new ALchar[nSampleSize];
		dev[0] = alcOpenDevice(NULL);
		if (NULL == dev[0])
		{
			fprintf(stderr, "No microphone found, please restart application , or continue streaming with out sound\n");
			boost::this_thread::sleep(boost::posix_time::seconds(9999999));
			cin.get();
			return;
		}

		ctx = alcCreateContext(dev[0], NULL);
		alcMakeContextCurrent(ctx);
		int i = -1;
		string bufferString[99];
		const ALchar *pDeviceList = alcGetString(NULL, ALC_CAPTURE_DEVICE_SPECIFIER);
		const ALCchar *bufferList[99];
		if (pDeviceList)
		{
			printf("\nLet us select audio device\n");
			printf("Available Capture Devices are:\n");
			i = 0;
			while (*pDeviceList)
			{
				bufferList[i] = pDeviceList;
				bufferString[i] += pDeviceList;
				cout << i <<") " << bufferString[i] << endl;
				pDeviceList += strlen(pDeviceList) + 1;
				i++;
			}
		}

		//Get the name of the 'default' capture device
		//szDefaultCaptureDevice = alcGetString(NULL, ALC_CAPTURE_DEFAULT_DEVICE_SPECIFIER);
		//printf("\nDefault Capture Device is '%s'\n\n", szDefaultCaptureDevice);

		int SelectedIndex = 999;
		if (i <= 0)
		{
			cout <<"No devices found. \n " << endl;
			//cout <<"Please restart application." << endl;
			//cin.get();
			//Sleep(999999);
			SelectedIndex = 999;
		}
		else
			if (i == 1)
			{
				cout <<"Default device will be used" << std::endl;
				SelectedIndex = 0;
				encoder.hasAudio = true;
			}
			else
			{
				while(SelectedIndex > i-1 || SelectedIndex < 0)
				{
					try
					{
						std::cout << "please input index from 0 to " << i - 1 << std::endl;
						std::string s;
						std::getline(cin, s, '\n');
						SelectedIndex = boost::lexical_cast<int>(s);
					}
					catch(std::exception&)
					{
						SelectedIndex = 999;
					}
				}
			}

		if (SelectedIndex == 999)
		{
			encoder.hasAudio = false;
		}
		else
		{
			encoder.hasAudio = true;
			alDistanceModel(AL_NONE);
			dev[0] = alcCaptureOpenDevice(bufferList[SelectedIndex], audio_sample_rate, AL_FORMAT_MONO16, nSampleSize/2);
			alcCaptureStart(dev[0]);
		}
	}
}

void init_ffmpeg(string container, int w, int h, int fps)
{
	if (!encoder.hasAudio && !encoder.hasVideo)
	{
		cout << "\nNo audio, and no video data found.\n Please close application.\nConnect some capturing device.\nRestart application\n";
		cin.get();
		boost::this_thread::sleep(boost::posix_time::seconds(9999999));
		cin.get();
	}
	encoder.SetConstants(fps, video_width, video_height, audio_sample_rate, stream_bitrate);

name:
	int encoderName = encoder.ConnectUserToUrl(username);
	if (encoderName == 0)
	{
		//printf("Cannot open stream for selected name\n");
		get_name();
		int encoderServer = encoder.ConnectToServer(server_url) ;
		goto name;
	}

	if (encoder.InitUrl(container, server_url, username) == -10)
	{
		cout << "\nNo audio, and no video data found.\n Please close application.\nConnect some capturing device.\nRestart application\n";
		cin.get();
		boost::this_thread::sleep(boost::posix_time::seconds(9999999));
	}

	int bufferImgSize = avpicture_get_size(PIX_FMT_BGR24, w, h);

	frame = avcodec_alloc_frame();
	uint8_t* frameBuffer = (uint8_t*)av_mallocz(bufferImgSize);
	avpicture_fill((AVPicture*)frame, frameBuffer, PIX_FMT_BGR24, w, h);

	readyFrame = avcodec_alloc_frame();
	uint8_t* readyFrameBuffer = (uint8_t*)av_mallocz(bufferImgSize);
	avpicture_fill((AVPicture*)readyFrame, readyFrameBuffer, PIX_FMT_BGR24, w, h);

	sample = new char[nSampleSize];
}

void capture_frame(int w, int h, char* buffer, int bytespan)
{
	if (flag_generate_video)
	{
		cvResize(CVframe, CVframeWithText);
		ptime now = second_clock::local_time();
		cvPutText(CVframeWithText, to_simple_string(now.time_of_day()).c_str(), cvPoint(0, (h / 2 + 10)), &font, CV_RGB(1, 1, 1));
		for (int i = 0; i < w * 4 * h; i = i + 4)
		{
			buffer[0] = CVframeWithText->imageData[i];
			buffer[1] = CVframeWithText->imageData[i + 1];
			buffer[2] = CVframeWithText->imageData[i + 2];
			buffer += 3;
		}
		//if (rainbow)
		//{
		//	int wxh = w * h;
		//	static float seed = 1.0;
		//	for (int i = 0; i < h; i++)
		//	{
		//		char* line = buffer + i * bytespan;
		//		for (int j = 0; j < w; j ++)
		//		{
		//			// RGB
		//			line[0] = 255 * sin(((float)i / wxh * seed) * 3.14);
		//			line[1] = 255 * cos(((float)j / wxh * seed) * 3.14);
		//			line[2] = 255 * sin(((float)(i + j) / wxh * seed) * 3.14);
		//			line += 3;
		//		}
		//	}
		//	seed = seed + 2.2;
		//}
	}
	else
	{
		CVframe = cvQueryFrame(capture);
		if (!CVframe)
		{
			printf("No CV frame captured!\n");
			cin.get();
		}

		cvResize(CVframe, destination);
		if (flag_lsd)
		{
			IplImage *destinationForLSD = cvCreateImage(cvSize(w, h), IPL_DEPTH_8U, 1);
			cvCvtColor(destination, destinationForLSD, CV_RGB2GRAY);

			image_double lsdImage;
			ntuple_list lsdOut;
			lsdImage = new_image_double(w, h);

			for (int x = 0; x < w; x++)
				for (int y = 0; y < h; y++)
					lsdImage->data[x + y * lsdImage->xsize] = cvGetReal2D(destinationForLSD, y, x);

			// call LSD
			lsdOut = lsd(lsdImage);

			for (unsigned int i = 0; i < lsdOut->size; i++)
			{
				CvPoint pt1 = { (int)lsdOut->values[i * lsdOut->dim + 0], (int)lsdOut->values[i * lsdOut->dim + 1] };
				CvPoint pt2 = { (int)lsdOut->values[i * lsdOut->dim + 2], (int)lsdOut->values[i * lsdOut->dim + 3] };
				cvLine(destination, pt1, pt2, CV_RGB(240, 255, 255), 1, CV_AA, 0);
			}
			cvReleaseImage(&destinationForLSD);
			free_image_double(lsdImage);
			free_ntuple_list(lsdOut);
		}

		for (int i = 0; i < destination->imageSize; i = i + 3)
		{
			buffer[2] = destination->imageData[i];
			buffer[1] = destination->imageData[i + 1];
			buffer[0] = destination->imageData[i + 2];
			buffer += 3;
		}

		//cvSplit(destination, bluechannel, greenchannel, redchannel, NULL);
		//for(int y = 0; y < destination->height; y++)
		//{
		//	char* line = buffer + y * bytespan;
		//	for(int x = 0; x < destination->width; x++)
		//	{
		// 		line[0] = cvGetReal2D(redchannel, y, x);
		// 		line[1] = cvGetReal2D(greenchannel, y, x);
		// 		line[2] = cvGetReal2D(bluechannel, y, x);
		// 		line += 3;
		//	}
		//}

		//for (int i = 0; i < w * h * 3; ++i) {
		// 		buffer[i] = destination->imageData;
		//}
	}
}

char* capture_sample()
{
	if (flag_generate_audio)
	{
		for (int i = 0; i < nSampleSize / nBlockAlign; i++)
			Buffer [i] = die();
	}
	else
	{
		// Check how much audio data has been captured (note that 'val' is the number of frames, not bytes)
		alcGetIntegerv(dev[0], ALC_CAPTURE_SAMPLES, 1, &iSamplesAvailable);
		// When we would have enough data to fill our BUFFERSIZE byte buffer, will grab the samples, so now we should wait
		while (iSamplesAvailable < (nSampleSize / nBlockAlign) - 1) // -1 was added to make code run on Mac OS X, potential bug
			alcGetIntegerv(dev[0], ALC_CAPTURE_SAMPLES, 1, &iSamplesAvailable);
		// Consume Samples
		alcCaptureSamples(dev[0], Buffer, (nSampleSize / nBlockAlign) - 1);
	}
	return (char*)Buffer;
}

void release_opencv()
{
	if (!flag_generate_video)
	{
		cvReleaseCapture(&capture);
		//cvReleaseImage(&destination);
		//cvReleaseImage(&CVframe);
	}
}

void release_openal()
{
}

void release_ffmpeg()
{
	encoder.Finish();

	av_free(frame->data[0]);
	av_free(frame);

	av_free(readyFrame->data[0]);
	av_free(readyFrame);

	delete[] sample;
	sample = NULL;
}

void capture_frame_loop()
{
	if (encoder.hasVideo)
	{
		while (true)
		{
			timerForCaptureFame.restart();
			capture_frame(video_width, video_height, (char*)frame->data[0], frame->linesize[0]);
			AVFrame* swap = frame;
			frame = readyFrame;
			readyFrame = swap;
			spendedTimeForCaptureFame = (int64_t)timerForCaptureFame.elapsed();
			if (spendedTimeForCaptureFame < desiredTimeForCaptureFame)
				boost::this_thread::sleep(boost::posix_time::milliseconds(desiredTimeForCaptureFame - spendedTimeForCaptureFame));
		}
	}
}

void save_frame_loop()
{
	while (true)
	{
		timerForMain.restart();
		if (!encoder.hasVideo)
			if (!encoder.AddFrame(capture_sample(), nSampleSize))
				printf("Cannot write frame!\n");

		if (!encoder.hasAudio)
			if (!encoder.AddFrame(readyFrame))
				printf("Cannot write frame!\n");
		
		if (encoder.hasAudio && encoder.hasVideo)
			if (!encoder.AddFrame(readyFrame, capture_sample(), nSampleSize))
				printf("Cannot write frame!\n");
		
		if (!encoder.hasAudio && !encoder.hasVideo)
		{
			printf("No data to encode");
			break;
		}

		spendedTimeForMain = (int64_t)timerForMain.elapsed();
		if(spendedTimeForMain < desiredTimeForMain)
			boost::this_thread::sleep(boost::posix_time::milliseconds(desiredTimeForMain - spendedTimeForMain));
	}
}

int main(int argc, char* argv[])
{
	audio_capture_device = -1;
	audio_sample_rate = 44100;
	container = "flv";
	flag_disable_audio = false;
	flag_disable_video = false;
	flag_generate_audio = false;
	flag_generate_video = false;
	flag_lsd = false;
	server_url = "";
	stream_bitrate = 1048576;
	username = "";
	video_capture_device = -1;
	video_frame_rate = 15;
	video_height = 720;
	video_width = 1280;

	for (int i = 1; i < argc; i++)
	{
		std::string arg = string(argv[i]);
		int pos = arg.find("=");
		if (std::string::npos != pos)
		{
			string key = arg.substr(0, pos);
			string value = arg.substr(pos + 1, arg.length() - pos - 1);

			if (key == "--audio-capture-device")
				audio_capture_device = atoi(value.c_str());
			if (key == "--audio-sample-rate")
				audio_sample_rate = atoi(value.c_str());
			if (key == "--container")
				container = value;
			if (key == "--server-url")
				server_url = value;
			if (key == "--stream-bitrate")
				stream_bitrate = atoi(value.c_str());
			if (key == "--username")
				username = value;
			if (key == "--video-capture-device")
				video_capture_device = atoi(value.c_str());
			if (key == "--video-frame-rate")
				video_frame_rate = atoi(value.c_str());
			if (key == "--video-height")
				video_height = atoi(value.c_str());
			if (key == "--video-width")
				video_width = atoi(value.c_str());
		}
		else
		{
			if (arg == "--disable-audio")
				flag_disable_audio = true;
			if (arg == "--disable-video")
				flag_disable_video = true;
			if (arg == "--generate-audio")
				flag_generate_audio = true;
			if (arg == "--generate-video")
				flag_generate_video = true;
			if (arg == "--lsd")
				flag_lsd = true;
			if (arg == "--robot")
			{
				flag_generate_audio = true;
				flag_generate_video = true;
			}
		}
	}

	video_height = get_evan(video_height);
	video_width = get_evan(video_width);

	desiredTimeForCaptureFame = (int64_t)(1000.0f / video_frame_rate);
	desiredTimeForMain = (int64_t)(1000.0f / video_frame_rate);

	if(server_url == "")
	{
		cout << "Warning: No Cloud Observer server url found!" << endl;
		get_server();
	}
	else
	{
		replace_or_merge(server_url, "http://", "tcp://");
	}

server:
	int encoderServer = encoder.ConnectToServer(server_url);
	if (encoderServer == -1)
	{
		//cout << "Cannot open stream URL\n";
		get_server();
		goto server;
	}

	if (username == "")
	{
		cout << "Please provide us with your user name" << endl;
		get_name();

		/*username += "robot";
		srand((unsigned)time(0));
		int random_integer = rand();
		username += boost::lexical_cast<string>(random_integer);*/
	}

	init_opencv();
	init_openal(video_frame_rate);
	init_ffmpeg(container, video_width, video_height, video_frame_rate);

	boost::thread workerThread(capture_frame_loop);
	boost::this_thread::sleep(boost::posix_time::milliseconds(200));
	boost::thread workerThread2(save_frame_loop);

	string quite;
	while (quite != "exit")
	{
		cout << "\nInput 'exit' to quite" << endl;
		cin >> quite;
		//cout << endl;
		boost::this_thread::sleep(boost::posix_time::milliseconds(250));
	}

	workerThread2.interrupt();
	workerThread.interrupt();
	boost::this_thread::sleep(boost::posix_time::milliseconds(250));

	release_opencv();
	release_openal();
	release_ffmpeg();

	return 0;
}
