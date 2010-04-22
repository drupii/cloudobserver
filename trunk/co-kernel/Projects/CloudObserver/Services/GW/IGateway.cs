﻿using System.ServiceModel;

namespace CloudObserver.Services.GW
{
    [ServiceContract]
    public interface IGateway// : IService
    {
        //[OperationContract]
        //string GetWorkBlock();

        [OperationContract]
        string IWannaRead(int[] contentIds);

        [OperationContract]
        string IWannaWrite(int contentId);
    }
}
 