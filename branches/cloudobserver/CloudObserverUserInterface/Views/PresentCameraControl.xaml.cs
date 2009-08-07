﻿using System;
using System.IO;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Documents;
using System.Windows.Ink;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Media.Animation;
using System.Windows.Shapes;
using System.Windows.Threading;
using System.ServiceModel;
using CloudObserverUserInterface.BroadcastServiceReference;
using CloudObserverUserInterface.CloudObserverAuthorizationServiceReference;

namespace CloudObserverUserInterface
{
	public partial class CameraControl : UserControl
	{
        private const int MAX_REQUESTS = 5;

        private int fps = 0;
        private int requests = 0;
        private DispatcherTimer refreshTimer;
        private DispatcherTimer fpsTimer;
        private MessageWindow errorMessageWindow;

        BroadcastServiceContractClient broadcastServiceClient;
     
		public CameraControl()
		{  //	opt.Opacity = 0;
        
            broadcastServiceClient = new BroadcastServiceContractClient(new BasicHttpBinding(), new EndpointAddress("http://93.100.45.201:9000/BroadcastService"));
            broadcastServiceClient.ReadFrameCompleted += new EventHandler<ReadFrameCompletedEventArgs>(client_ReadFrameCompleted);
            refreshTimer = new DispatcherTimer();
            refreshTimer.Interval = TimeSpan.FromMilliseconds(1000 / 60);
            refreshTimer.Tick += new EventHandler(refreshTimer_Tick);
            refreshTimer.Start();
            fpsTimer = new DispatcherTimer();
            fpsTimer.Interval = TimeSpan.FromSeconds(1);
            fpsTimer.Tick += new EventHandler(fpsTimer_Tick);
            fpsTimer.Start();
            this.Height = 300;

			InitializeComponent();
		}

        private void fpsTimer_Tick(object sender, EventArgs e)
        {
            TextBlockFPS.Text = "FPS: " + fps;
            fps = 0;
        }

        private void refreshTimer_Tick(object sender, EventArgs e)
        {
            if (requests < MAX_REQUESTS)
            {
                broadcastServiceClient.ReadFrameAsync((int)NumericUpDownCameraID.Value);
                requests++;
            }
        }

        private void client_ReadFrameCompleted(object sender, ReadFrameCompletedEventArgs e)
        {
            if (e.Error != null)
            {
                refreshTimer.Stop();
                if (errorMessageWindow == null)
                {
                    errorMessageWindow = new MessageWindow("Can't access broadcast service.", "Error", new TimeSpan(0, 0, 2));
                    errorMessageWindow.Closed += new EventHandler(errorMessageWindow_Closed);
                    errorMessageWindow.Show();
                }
                return;
            }
            fps++;
            requests--;
            byte[] receivedImage = e.Result;
            BitmapImage bitmapImage = new BitmapImage();
            bitmapImage.SetSource(new MemoryStream(receivedImage));
            ImageFrame.Source = bitmapImage;
        }

        private void errorMessageWindow_Closed(object sender, EventArgs e)
        {
            errorMessageWindow = null;
        }

        private void NumericUpDownRefreshRate_ValueChanged(object sender, RoutedPropertyChangedEventArgs<double> e)
        {
            refreshTimer.Interval = TimeSpan.FromMilliseconds(1000 / e.NewValue);
        }

    

        private void mEnter(object sender, System.Windows.Input.MouseEventArgs e)
        {
   optbut.Opacity = 99;     // TODO: Add event handler implementation here.
        }

        private void mLeave(object sender, System.Windows.Input.MouseEventArgs e)
        {
          optbut.Opacity = 20; 	// TODO: Add event handler implementation here.
        }

        private void winOptOpen(object sender, System.Windows.RoutedEventArgs e)
        {
      opt.Opacity = 99;			// TODO: Add event handler implementation here.
        }

        private void okClick(object sender, System.Windows.RoutedEventArgs e)
        {
        	opt.Opacity = 0;
			optbut.Opacity = 99;// TODO: Add event handler implementation here.
        }
	}
}