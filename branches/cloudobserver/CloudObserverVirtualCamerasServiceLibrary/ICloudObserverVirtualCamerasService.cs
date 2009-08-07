﻿using System;
using System.ServiceModel;

namespace CloudObserverVirtualCamerasServiceLibrary
{
    [ServiceContract]
    public interface ICloudObserverVirtualCamerasService
    {
        [OperationContract]
        void SetSource(int cameraID, string source, string SourceType);

        [OperationContract]
        void SetCredentials(int cameraID, string userName, string password);

        [OperationContract]
        void SetFPS(int cameraID, int fps);

        [OperationContract]
        void StartBroadcasting(int cameraID);

        [OperationContract]
        void StopBroadcasting(int cameraID);

        [OperationContract]
        int GetFramesCounter(int cameraID);
    }
}