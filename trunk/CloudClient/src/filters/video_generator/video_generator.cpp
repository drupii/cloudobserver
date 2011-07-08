#include "video_generator.h"

video_generator::video_generator(int video_width, int video_height, int video_frame_rate, std::string username)
{
	this->width = video_width;
	this->height = video_height;
	this->frame_rate = video_frame_rate;

	cvInitFont(&font, CV_FONT_HERSHEY_DUPLEX, 2, 1, 0.0, 3, CV_AA);

	this->blank_frame = cvCreateImage(cvSize(this->width, this->height), 8, 4);
	this->resized_frame = cvCreateImage(cvSize(this->width, this->height), 8, 4);
	cvRectangle(this->blank_frame, cvPoint(0, 0), cvPoint(this->width, this->height), CV_RGB(0, 254, 53), CV_FILLED);
	cvPutText(this->blank_frame, username.c_str(), cvPoint(0, this->height - 10), &font, CV_RGB(1, 1, 1));

	this->frame = avcodec_alloc_frame();
	uint8_t* frame_buffer = (uint8_t*)av_mallocz(avpicture_get_size(PIX_FMT_BGR24, this->width, this->height));
	avpicture_fill((AVPicture*)frame, frame_buffer, PIX_FMT_BGR24, this->width, this->height);

	this->generation_thread = NULL;

	this->video_encoder_block = NULL;
}

video_generator::~video_generator()
{
	av_free(this->frame->data[0]);
	av_free(this->frame);
}

void video_generator::connect(video_encoder* video_encoder_block)
{
	this->video_encoder_block = video_encoder_block;
}

void video_generator::disconnect()
{
	this->video_encoder_block = NULL;
}

void video_generator::start()
{
	this->generation_thread = new boost::thread(&video_generator::generation_loop, this);
}

void video_generator::stop()
{
	this->generation_thread->interrupt();
}

void video_generator::generation_loop()
{
	while (true)
	{
		cvResize(this->blank_frame, this->resized_frame);
		boost::posix_time::ptime now = boost::date_time::second_clock<boost::posix_time::ptime>::local_time();
		cvPutText(this->resized_frame, boost::posix_time::to_simple_string(now.time_of_day()).c_str(),
			cvPoint(0, this->height / 2 + 10), &font, CV_RGB(1, 1, 1));

		char* buffer = (char*)this->frame->data[0];
		for (int i = 0; i < 4 * this->width * this->height; i += 4)
		{
			buffer[0] = this->resized_frame->imageData[i];
			buffer[1] = this->resized_frame->imageData[i + 1];
			buffer[2] = this->resized_frame->imageData[i + 2];
			buffer += 3;
		}

		this->video_encoder_block->send(this->frame);

		boost::this_thread::sleep(boost::posix_time::milliseconds(1000 / this->frame_rate));
	}
}