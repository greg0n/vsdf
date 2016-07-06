#include "COpenCV.h"

COpenCV::COpenCV(QObject *parent) :
    QObject(parent)
{

}

QImage COpenCV::cvtCvMat2QImage(const cv::Mat & image)
{
	QImage qtemp;
	if(!image.empty() && image.depth() == CV_8U)
	{
		const unsigned char * data = image.data;
		qtemp = QImage(image.cols, image.rows, QImage::Format_RGB32);
		for(int y = 0; y < image.rows; ++y, data += image.cols*image.elemSize())
		{
			for(int x = 0; x < image.cols; ++x)
			{
				QRgb * p = ((QRgb*)qtemp.scanLine (y)) + x;
				*p = qRgb(data[x * image.channels()+2], data[x * image.channels()+1], data[x * image.channels()]);
			}
		}
	}
	else if(!image.empty() && image.depth() != CV_8U)
	{
		QMessageBox::warning(0, "Format failed", "Wrong image format, must be 8 bits");
	}
	return qtemp;
}

cv::Mat COpenCV::cvtQImage2CvMat(const QImage & image)
{
	cv::Mat cvImage;
	if(!image.isNull() && image.depth() == 32 && image.format() == QImage::Format_RGB32)
	{
		// assume RGB (3 channels)
		int channels = 3;
		cvImage = cv::Mat(image.height(), image.width(), CV_8UC3);
		unsigned char * data = cvImage.data;
		for(int y = 0; y < image.height(); ++y, data+=cvImage.cols*cvImage.elemSize())
		{
			for(int x = 0; x < image.width(); ++x)
			{
				QRgb rgb = image.pixel(x, y);
				data[x * channels+2] = qRed(rgb); //r
				data[x * channels+1] = qGreen(rgb); //g
				data[x * channels] = qBlue(rgb); //b
			}
		}
	}
	else if(!image.isNull() && image.depth() == 8 && image.format() == QImage::Format_Indexed8)
	{
		cvImage = cv::Mat(image.height(), image.width(), CV_8UC1); //1 channel
		unsigned char * data = cvImage.data;
		for(int y = 0; y < image.height(); ++y, data+=cvImage.cols*cvImage.elemSize())
			for(int x = 0; x < image.width(); ++x)
				data[x] = data[x+1] = data[x+2] = qGray(image.pixel(x, y));
	}
	else
		QMessageBox::warning(0, "Format failed", "Wrong image format, must be 8 or 32 bits");
	
	return cvImage;
}

QImage COpenCV::toGrayScale(QImage qimage)
{
	//QtConcurrent::run(qimage, convertToFormat, QImage::Format_Mono, Qt::MonoOnly);
	return qimage.convertToFormat(QImage::Format_Mono, Qt::MonoOnly);
	//return qimage;
}

QImage COpenCV::toNegative(QImage qimage)
{
	qimage.invertPixels();
	return qimage;
}

QImage COpenCV::findAndDrawCircles(QImage qimage, QPoint *point, int *rad)
{
	QImage imageProcessed, temp;
	cv::Mat src, src_gray;
	src = cvtQImage2CvMat(qimage);
	temp = cvtCvMat2QImage(src);
	src = cvtQImage2CvMat(temp);

	/// Convert it to gray
	cv::cvtColor( src, src_gray, CV_BGR2GRAY );

	/// Reduce the noise so we avoid false circle detection
	cv::GaussianBlur( src_gray, src_gray, cv::Size(9, 9), 2, 2 );

	cv::vector<cv::Vec3f> circles;

	/// Apply the Hough Transform to find the circles
	cv::HoughCircles( src_gray, circles, CV_HOUGH_GRADIENT, 2, 20, 90, 110, 40, 160 );

	//QMessageBox messageBox;
	//messageBox.information(0,"VSDP",QString::number(circles.size()));

	/// Draw the circles detected
	for( size_t i = 0; i < circles.size(); i++ )
	{
		cv::Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
		int radius = cvRound(circles[i][2]);
		// circle center
		circle( src, center, 3, cv::Scalar(0,255,0), -1, 8, 0 );
		// circle outline
		circle( src, center, radius, cv::Scalar(0,0,255), 3, 8, 0 );
		*point = toQt(center); //zwrócenie wartoœci przez wskaŸnik
		*rad = radius;		   //zwrócenie wartoœci przez wskaŸnik
	}
	imageProcessed = cvtCvMat2QImage(src);
	return imageProcessed;
}
