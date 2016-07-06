#ifndef COPENCV_H
#define COPENCV_H

#include <QObject>
#include <QImage>
#include <QDebug>
#include <QtConcurrent>
#include <QMessageBox>
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"

class COpenCV : public QObject
{
    Q_OBJECT
public:
    explicit COpenCV(QObject *parent = 0);

    static QImage  toGrayScale(QImage qimage);
    static QImage  toNegative(QImage qimage);
    static QImage  findAndDrawCircles(QImage qimage, QPoint *point, int *rad);

private:
    static QImage cvtCvMat2QImage(const cv::Mat & image);
    static cv::Mat cvtQImage2CvMat(const QImage & image);

	static inline cv::Point toCv(QPoint& point)
	{
    	return cv::Point(point.x(), point.y());
	}

	static inline QPoint toQt(cv::Point& point)
	{
		return QPoint(point.x, point.y);
	}
signals:
    
public slots:
    
};

#endif // COPENCV_H
