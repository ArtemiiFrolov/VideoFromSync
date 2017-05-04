#include "VideoIntensityAnalyzer.h"

namespace video_util {
	VideoIntensityAnalyzer::VideoIntensityAnalyzer()
	{
	}


	VideoIntensityAnalyzer::~VideoIntensityAnalyzer()
	{
		Close();
	}

	void VideoIntensityAnalyzer::SetRegion(Region region)
	{
		region_ = region;
	}

	void VideoIntensityAnalyzer::SetTreshold(unsigned short threshold)
	{
		threshold_ = threshold;
	}

	void VideoIntensityAnalyzer::Open(std::string stream_name)
	{
		Close();
		capture_ = new cv::VideoCapture(stream_name);
	}

	void VideoIntensityAnalyzer::Close()
	{	
		if (capture_ != nullptr)
		{			
			delete capture_;
		}
	}

	bool VideoIntensityAnalyzer::IsOpened()
	{
		if (capture_ != nullptr)
		{
			return capture_->isOpened();
		}

		return false;
	}
}
