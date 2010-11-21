#include "stdafx.h"
#include <windows.h>
#include <stdio.h>
#include <iostream>

//#include <unistd.h>
//#include <stdlib.h>
//#include <Pthread.h>

#include <boost/thread.hpp>

using namespace std;

int ParseServerName()
{
#if 0
		if(outputUrl == ""){
		cout << "Warning: No Cloud Observer server url found!" << endl ;
		getAdress();
	}else{
		replace_or_merge(outputUrl, "http://", "tcp://");
	}
	
	int encoderServer = encoder.ConnectToServer(outputUrl) ;
	if (encoderServer == -1)
	{
		getAdress();
	goto server;
}
if(outputUserName == ""){
	cout << "Please provide us with your user name" << endl ;
	getName();
}
#endif
	return 0;
}
void ExitProgram() 
{
	string quite;
		while(quite != "exit")
	{
		cout << "Input 'exit' to quite" << endl;
		cin >> quite;
		Sleep(250);
	}
}

void CaptureLoop(void) 
{
	 //printf("thread_function started. Arg was %s\n", (char *)arg);
     // pause for 3 seconds
    // sleep(3);
     // exit and  return a message to another thread
     // that may be waiting for us to finish
     //pthread_exit ("thread one all done�);
	while(true)
	{
		boost::xtime xt;
		boost::xtime_get(&xt, boost::TIME_UTC);


		xt.sec +=1;

		boost::thread::sleep(xt);
	}
}

int main(int argc, char* argv[])
{
//	pthread_t capture_thread;

	//TODO getopt
	for(int i = 1; i<argc; i=i+2){
#if 0
		if(string(argv[i]) == "-camera") {cameraInt = atoi(argv[i+1]);} 
		if(string(argv[i]) == "-framerate" ){videoFrameRate = atoi(argv[i+1]);} 
		if(string(argv[i]) == "-width" ) {videoWidth = atoi(argv[i+1]);} 
		if(string(argv[i]) == "-height" ) {videoHeight = atoi(argv[i+1]);} 
		if(string(argv[i]) == "-microphone" ) {microphoneInt = atoi(argv[i+1]);} 
		if(string(argv[i]) == "-samplerate" ) {audioSampleRate = atoi(argv[i+1]);} 
		if(string(argv[i]) == "-server" ) {outputUrl = (argv[i+1]);} 
		if(string(argv[i]) == "-container" ) {outputContainer = (argv[i+1]);} 
		if(string(argv[i]) == "-nickname" ) {outputUserName = (argv[i+1]);} 
		if(string(argv[i]) == "-useLSD" ) {useLSD = atoi(argv[i+1]);} 
		if(string(argv[i]) == "-streamBitRate" ) {streamBitRate = atoi(argv[i+1]);} 
		if(string(argv[i]) == "-noMic" ) {noMic = atoi(argv[i+1]);} 
#endif
	}	
	Sleep(1000);
	//pthread_create (&capture_thread, NULL, capture_loop, (void*)NULL);

	boost::thread workerThread(&CaptureLoop);
	ExitProgram();
	workerThread.interrupt();


	workerThread.join();

	return 0;
}