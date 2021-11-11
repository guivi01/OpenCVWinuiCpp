#include "pch.h"
#include "MainWindow.xaml.h"
#if __has_include("MainWindow.g.cpp")
#include "MainWindow.g.cpp"
#endif

//using namespace winrt;
using namespace winrt::Microsoft::UI::Xaml;

// To learn more about WinUI, the WinUI project structure,
// and more about our project templates, see: http://aka.ms/winui-project-info.

namespace winrt::winui3_test::implementation
{
    DWORD WINAPI StartThread(LPVOID lParam)
    {
        MainWindow* pThis = (MainWindow*)lParam;
        pThis->CaptureThread();
        return 1;
    }

    MainWindow::MainWindow()
    {
        InitializeComponent();
        m_hThread = NULL;
        m_bThread = false;
    }

    int32_t MainWindow::MyProperty()
    {
        throw hresult_not_implemented();
    }

    void MainWindow::MyProperty(int32_t /* value */)
    {
        throw hresult_not_implemented();
    }

    winrt::Windows::Foundation::IAsyncAction MainWindow::CaptureThread()
    {
        winrt::apartment_context ui_thread; // Capture calling context.
        co_await winrt::resume_background();
        cv::VideoCapture capture;
        capture.open(0);
        if (capture.isOpened())
        {
            m_bThread = true;
            while (m_bThread)
            {
                cv::Mat frame;
                capture >> frame;

                cv::cvtColor(frame, frame, cv::COLOR_BGR2BGRA);

                co_await ui_thread; // Switch back to calling context.
                winrt::Microsoft::UI::Xaml::Media::Imaging::WriteableBitmap bmp(frame.cols, frame.rows);
                winrt::Windows::Storage::Streams::IBuffer buffer(bmp.PixelBuffer());
                unsigned char* dstPixels;
                
                using namespace winrt::Microsoft::UI::Xaml::Media;
                using namespace Windows::Foundation;
                using namespace Windows::Storage::Streams;
                using namespace Windows::Storage;
                using namespace Windows::Storage::Streams;
                using namespace winrt::Microsoft::UI::Xaml::Media::Imaging;
                using namespace Windows::Graphics::Imaging;
                using namespace Windows::Foundation::Collections;
                using namespace winrt::Microsoft::UI::Xaml;
                using namespace winrt::Microsoft::UI::Xaml::Controls;
                using namespace winrt::Microsoft::UI::Xaml::Controls::Primitives;
                using namespace winrt::Microsoft::UI::Xaml::Data;
                using namespace winrt::Microsoft::UI::Xaml::Input;
                using namespace winrt::Microsoft::UI::Xaml::Media;
                using namespace winrt::Microsoft::UI::Xaml::Navigation;
                using namespace winrt::impl;

                // Obtain IBufferByteAccess                
                winrt::com_ptr<IBufferByteAccess> pBufferByteAccess;
                IInspectable pBuffer((IInspectable) buffer);
                pBuffer.as( pBufferByteAccess);

                // Get pointer to pixel bytes
                pBufferByteAccess->Buffer(&dstPixels);
                memcpy(dstPixels, frame.data, frame.step.buf[1] * frame.cols * frame.rows);

                // Set the bitmap to the Image element
                this->myImage().Source( bmp);
                co_await winrt::resume_background();
            }
        }
        //CloseHandle(m_hThread);
        //m_hThread = NULL;
    }

    void MainWindow::myButton_Click(IInspectable const&, RoutedEventArgs const&)
    {
        //myButton().Content(box_value(L"Clicked"));
        if (m_bThread == false)// m_hThread == NULL)
        {
            CaptureThread();
            //m_hThread = CreateThread(NULL, NULL, StartThread, this, NULL, NULL);
        }
        else
        {
            m_bThread = false;
            //WaitForSingleObject(m_hThread, INFINITE);            
        }
    }
}
