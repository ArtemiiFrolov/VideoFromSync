#ifndef VIDEO_INTENSITY_ANALYZER_H_
#define VIDEO_INTENSITY_ANALYZER_H_

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <string>


namespace video_util {
	struct Region
	{
		unsigned short x;
		unsigned short y;
		unsigned short width;
		unsigned short height;
	};

	struct FrameResults
	{
		unsigned short 
	};

	class VideoIntensityAnalyzer
	{
		public:
			VideoIntensityAnalyzer();
			~VideoIntensityAnalyzer();

			void SetRegion(Region region);
			void SetTreshold(unsigned short threshold);
			void Open(std::string stream_name);
			void Close();
			bool IsOpened();
			FrameResults ProcessNextFrame();

		private:
			cv::VideoCapture* capture_;
			Region region_;
			unsigned short threshold_;
	};
}

#endif VIDEO_INTENSITY_ANALYZER_H_