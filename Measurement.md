A bit later here will be written some dependences beetwen size of images, buffer size, tipe of binding and time.

http://geekswithblogs.net/BizG/archive/2008/02/22/socket-errors-in-wcf-nettcpbinding.aspx
> | size | speed |
|:-----|:------|
> | 1000 | 149   |

> ## NetTCPBinding ##

> #### Buffered ####

> #### Buffer : 10000 ####

START\_OF\_SECTION: NetTCPBinding
> TransferMode : Buffered
> Average used time for transfer(camera->service->client)(milliseconds) : 149;
> Average picture size(B) : 1036854;
> Average data transfered(B) : 2073708;
> MaxBytePerRead attribute: 10000
END\_OF\_SECTION

START\_OF\_SECTION: NetTCPBinding
> TransferMode : Buffered
> Average used time for transfer(camera->service->client)(milliseconds) : 63;
> Average picture size(B) : 360054;
> Average data transfered(B) : 720108;
> MaxBytePerRead attribute: 10000
END\_OF\_SECTION

START\_OF\_SECTION: NetTCPBinding
> TransferMode : Buffered
> Average used time for transfer(camera->service->client)(milliseconds) : 27;
> Average picture size(B) : 90054;
> Average data transfered(B) : 180108;
> MaxBytePerRead attribute: 10000
END\_OF\_SECTION

> #### Buffer : 60000 ####

START\_OF\_SECTION: NetTCPBinding
> TransferMode : Buffered
> Average used time for transfer(camera->service->client)(milliseconds) : 148;
> Average picture size(B) : 1036854;
> Average data transfered(B) : 2073708;
> MaxBytePerRead attribute: 60000
END\_OF\_SECTION

START\_OF\_SECTION: NetTCPBinding
> TransferMode : Buffered
> Average used time for transfer(camera->service->client)(milliseconds) : 64;
> Average picture size(B) : 360054;
> Average data transfered(B) : 720108;
> MaxBytePerRead attribute: 60000
END\_OF\_SECTION

START\_OF\_SECTION: NetTCPBinding
> TransferMode : Buffered
> Average used time for transfer(camera->service->client)(milliseconds) : 25;
> Average picture size(B) : 90054;
> Average data transfered(B) : 180108;
> MaxBytePerRead attribute: 60000
END\_OF\_SECTION

> ## WSHttpBinding ##

> ### Buffered ###

> #### Buffer : 10000 ####

START\_OF\_SECTION: WSHttpBinding
> TransferMode : Buffered
> Average used time for transfer(camera->service->client)(milliseconds) : 170;
> Average picture size(B) : 1036854;
> Average data transfered(B) : 2073708;
> MaxBytePerRead attribute: 10000
END\_OF\_SECTION

START\_OF\_SECTION: WSHttpBinding
> TransferMode : Buffered
> Average used time for transfer(camera->service->client)(milliseconds) : 77;
> Average picture size(B) : 360054;
> Average data transfered(B) : 720108;
> MaxBytePerRead attribute: 10000
END\_OF\_SECTION

START\_OF\_SECTION: WSHttpBinding
> TransferMode : Buffered
> Average used time for transfer(camera->service->client)(milliseconds) : 31;
> Average picture size(B) : 90054;
> Average data transfered(B) : 180108;
> MaxBytePerRead attribute: 10000
END\_OF\_SECTION

> #### Buffer : 60000 ####

START\_OF\_SECTION: WSHttpBinding
> TransferMode : Buffered
> Average used time for transfer(camera->service->client)(milliseconds) : 159;
> Average picture size(B) : 1036854;
> Average data transfered(B) : 2073708;
> MaxBytePerRead attribute: 60000
END\_OF\_SECTION

START\_OF\_SECTION: WSHttpBinding
> TransferMode : Buffered
> Average used time for transfer(camera->service->client)(milliseconds) : 74;
> Average picture size(B) : 360054;
> Average data transfered(B) : 720108;
> MaxBytePerRead attribute: 60000
END\_OF\_SECTION

START\_OF\_SECTION: WSHttpBinding
> TransferMode : Buffered
> Average used time for transfer(camera->service->client)(milliseconds) : 33;
> Average picture size(B) : 90054;
> Average data transfered(B) : 180108;
> MaxBytePerRead attribute: 60000
END\_OF\_SECTION

> ## BasicHttpBinding ##

> ### Streamed ###

> #### Buffer : 10000 ####

START\_OF\_SECTION: BasicHttpBinding
> TransferMode : Streamed
> Average used time for transfer(camera->service->client)(milliseconds) : 200;
> Average picture size(B) : 1036854;
> Average data transfered(B) : 2073708;
> MaxBytePerRead attribute: 10000
END\_OF\_SECTION

START\_OF\_SECTION: BasicHttpBinding
> TransferMode : Streamed
> Average used time for transfer(camera->service->client)(milliseconds) : 75;
> Average picture size(B) : 360054;
> Average data transfered(B) : 720108;
> MaxBytePerRead attribute: 10000
END\_OF\_SECTION

START\_OF\_SECTION: BasicHttpBinding
> TransferMode : Streamed
> Average used time for transfer(camera->service->client)(milliseconds) : 38;
> Average picture size(B) : 90054;
> Average data transfered(B) : 180108;
> MaxBytePerRead attribute: 10000
END\_OF\_SECTION

> #### Buffer : 60000 ####

START\_OF\_SECTION: BasicHttpBinding
> TransferMode : Streamed
> Average used time for transfer(camera->service->client)(milliseconds) : 191;
> Average picture size(B) : 1036854;
> Average data transfered(B) : 2073708;
> MaxBytePerRead attribute: 60000
END\_OF\_SECTION

START\_OF\_SECTION: BasicHttpBinding
> TransferMode : Streamed
> Average used time for transfer(camera->service->client)(milliseconds) : 76;
> Average picture size(B) : 360054;
> Average data transfered(B) : 720108;
> MaxBytePerRead attribute: 60000
END\_OF\_SECTION

START\_OF\_SECTION: BasicHttpBinding
> TransferMode : Streamed
> Average used time for transfer(camera->service->client)(milliseconds) : 35;
> Average picture size(B) : 90054;
> Average data transfered(B) : 180108;
> MaxBytePerRead attribute: 60000
END\_OF\_SECTION

> ### Buffered ###

> #### Buffer : 10000 ####

START\_OF\_SECTION: BasicHttpBinding
> TransferMode : Buffered
> Average used time for transfer(camera->service->client)(milliseconds) : 168;
> Average picture size(B) : 1036854;
> Average data transfered(B) : 2073708;
> MaxBytePerRead attribute: 10000
END\_OF\_SECTION

START\_OF\_SECTION: BasicHttpBinding
> TransferMode : Buffered
> Average used time for transfer(camera->service->client)(milliseconds) : 66;
> Average picture size(B) : 360054;
> Average data transfered(B) : 720108;
> MaxBytePerRead attribute: 10000
END\_OF\_SECTION

START\_OF\_SECTION: BasicHttpBinding
> TransferMode : Buffered
> Average used time for transfer(camera->service->client)(milliseconds) : 31;
> Average picture size(B) : 90054;
> Average data transfered(B) : 180108;
> MaxBytePerRead attribute: 10000
END\_OF\_SECTION

> #### Buffer : 60000 ####

START\_OF\_SECTION: BasicHttpBinding
> TransferMode : Buffered
> Average used time for transfer(camera->service->client)(milliseconds) : 164;
> Average picture size(B) : 1036854;
> Average data transfered(B) : 2073708;
> MaxBytePerRead attribute: 60000
END\_OF\_SECTION

START\_OF\_SECTION: BasicHttpBinding
> TransferMode : Buffered
> Average used time for transfer(camera->service->client)(milliseconds) : 63;
> Average picture size(B) : 360054;
> Average data transfered(B) : 720108;
> MaxBytePerRead attribute: 60000
END\_OF\_SECTION

START\_OF\_SECTION: BasicHttpBinding
> TransferMode : Buffered
> Average used time for transfer(camera->service->client)(milliseconds) : 31;
> Average picture size(B) : 90054;
> Average data transfered(B) : 180108;
> MaxBytePerRead attribute: 60000
END\_OF\_SECTION

> ## IPC Binding ##

> ### Buffered ###

> #### Buffer : 10000 ####

START\_OF\_SECTION: IPC : NetNamedPipeBinding
> TransferMode : Buffered
> Average used time for transfer(camera->service->client)(milliseconds) : 112;
> Average picture size(B) : 1036854;
> Average data transfered(B) : 2073708;
> MaxBytePerRead attribute: 10000
END\_OF\_SECTION

START\_OF\_SECTION: IPC : NetNamedPipeBinding
> TransferMode : Buffered
> Average used time for transfer(camera->service->client)(milliseconds) : 47;
> Average picture size(B) : 360054;
> Average data transfered(B) : 720108;
> MaxBytePerRead attribute: 10000
END\_OF\_SECTION

START\_OF\_SECTION: IPC : NetNamedPipeBinding
> TransferMode : Buffered
> Average used time for transfer(camera->service->client)(milliseconds) : 21;
> Average picture size(B) : 90054;
> Average data transfered(B) : 180108;
> MaxBytePerRead attribute: 10000
END\_OF\_SECTION

> #### Buffer : 60000 ####

START\_OF\_SECTION: IPC : NetNamedPipeBinding
> TransferMode : Buffered
> Average used time for transfer(camera->service->client)(milliseconds) : 109;
> Average picture size(B) : 1036854;
> Average data transfered(B) : 2073708;
> MaxBytePerRead attribute: 60000
END\_OF\_SECTION

START\_OF\_SECTION: IPC : NetNamedPipeBinding
> TransferMode : Buffered
> Average used time for transfer(camera->service->client)(milliseconds) : 45;
> Average picture size(B) : 360054;
> Average data transfered(B) : 720108;
> MaxBytePerRead attribute: 60000
END\_OF\_SECTION

START\_OF\_SECTION: IPC : NetNamedPipeBinding
> TransferMode : Buffered
> Average used time for transfer(camera->service->client)(milliseconds) : 21;
> Average picture size(B) : 90054;
> Average data transfered(B) : 180108;
> MaxBytePerRead attribute: 60000
END\_OF\_SECTION


> ### Streamed ###

> #### Buffer : 10000 ####

START\_OF\_SECTION: IPC : NetNamedPipeBinding
> TransferMode : Streamed
> Average used time for transfer(camera->service->client)(milliseconds) : 129;
> Average picture size(B) : 1036854;
> Average data transfered(B) : 2073708;
> MaxBytePerRead attribute: 10000
END\_OF\_SECTION

START\_OF\_SECTION: IPC : NetNamedPipeBinding
> TransferMode : Streamed
> Average used time for transfer(camera->service->client)(milliseconds) : 54;
> Average picture size(B) : 360054;
> Average data transfered(B) : 720108;
> MaxBytePerRead attribute: 10000
END\_OF\_SECTION

START\_OF\_SECTION: IPC : NetNamedPipeBinding
> TransferMode : Streamed
> Average used time for transfer(camera->service->client)(milliseconds) : 27;
> Average picture size(B) : 90054;
> Average data transfered(B) : 180108;
> MaxBytePerRead attribute: 10000
END\_OF\_SECTION

> #### Buffer : 60000 ####

START\_OF\_SECTION: IPC : NetNamedPipeBinding
> TransferMode : Streamed
> Average used time for transfer(camera->service->client)(milliseconds) : 126;
> Average picture size(B) : 1036854;
> Average data transfered(B) : 2073708;
> MaxBytePerRead attribute: 60000
END\_OF\_SECTION

START\_OF\_SECTION: IPC : NetNamedPipeBinding
> TransferMode : Streamed
> Average used time for transfer(camera->service->client)(milliseconds) : 53;
> Average picture size(B) : 360054;
> Average data transfered(B) : 720108;
> MaxBytePerRead attribute: 60000
END\_OF\_SECTION

START\_OF\_SECTION: IPC : NetNamedPipeBinding
> TransferMode : Streamed
> Average used time for transfer(camera->service->client)(milliseconds) : 26;
> Average picture size(B) : 90054;
> Average data transfered(B) : 180108;
> MaxBytePerRead attribute: 60000
END\_OF\_SECTION