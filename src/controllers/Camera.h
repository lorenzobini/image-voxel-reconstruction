/*
 * Camera.h
 *
 *  Created on: Nov 13, 2013
 *      Author: coert
 */

#ifndef CAMERA_H_
#define CAMERA_H_

#include <opencv2/core/core.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <string>
#include <vector>

namespace nl_uu_science_gmt
{

#define MAIN_WINDOW "Checkerboard Marking"

class Camera
{
	static std::vector<cv::Point>* m_BoardCorners;  // marked checkerboard corners

	bool m_initialized;                             // Is this camera successfully initialized

	const std::string m_data_path;                  // Path to data directory
	const std::string m_cam_props_file;             // Camera properties filename
	const int m_id;                                 // Camera ID

	std::vector<cv::Mat> m_bg_hsv_channels;          // Background HSV channel images
	cv::Mat m_foreground_image;                      // This camera's foreground image (binary)

	cv::VideoCapture m_video;                        // Video reader

	cv::Size m_plane_size;                           // Camera's FoV size
	long m_frame_amount;                             // Amount of frames in this camera's video

	cv::Mat m_camera_matrix;                         // Camera matrix (3x3)
	cv::Mat m_distortion_coeffs;                     // Distortion vector (5x1)
	cv::Mat m_rotation_values;                       // Rotation vector (3x1)
	cv::Mat m_translation_values;                    // Translation vector (3x1)

	float m_fx, m_fy, m_cx, m_cy;                   // Focal lenghth (fx, fy), camera center (cx, cy)

	cv::Mat m_rt;                                    // R matrix
	cv::Mat m_inverse_rt;                            // R's inverse matrix

	cv::Point3f m_camera_location;                   // Camera location in the 3D space
	std::vector<cv::Point3f> m_camera_plane;         // Camera plane of view
	std::vector<cv::Point3f> m_camera_floor;         // Projection of the camera itself onto the ground floor view

	cv::Mat m_frame;                                 // Current video frame (image)

	static void onMouse(int, int, int, int, void*);
	void initCamLoc();
	inline void camPtInWorld();

	cv::Point3f ptToW3D(const cv::Point &);
	cv::Point3f cam3DtoW3D(const cv::Point3f &);

public:
	Camera(const std::string &, const std::string &, int);
	virtual ~Camera();

	bool initialize();

	cv::Mat& advanceVideoFrame();
	cv::Mat& getVideoFrame(int);
	void setVideoFrame(int);

	static bool detExtrinsics(const std::string &, const std::string &, const std::string &, const std::string &);

	static cv::Point projectOnView(const cv::Point3f &, const cv::Mat &, const cv::Mat &, const cv::Mat &, const cv::Mat &);
	cv::Point projectOnView(const cv::Point3f &);

	const std::string& getCamPropertiesFile() const
	{
		return m_cam_props_file;
	}

	const std::string& getDataPath() const
	{
		return m_data_path;
	}

	const int getId() const
	{
		return m_id;
	}

	const cv::VideoCapture& getVideo() const
	{
		return m_video;
	}

	void setVideo(const cv::VideoCapture& video)
	{
		m_video = video;
	}

	long getFramesAmount() const
	{
		return m_frame_amount;
	}

	const std::vector<cv::Mat>& getBgHsvChannels() const
	{
		return m_bg_hsv_channels;
	}

	bool isInitialized() const
	{
		return m_initialized;
	}

	const cv::Size& getSize() const
	{
		return m_plane_size;
	}

	const cv::Mat& getForegroundImage() const
	{
		return m_foreground_image;
	}

	void setForegroundImage(const cv::Mat& foregroundImage)
	{
		m_foreground_image = foregroundImage;
	}

	const cv::Mat& getFrame() const
	{
		return m_frame;
	}

	const std::vector<cv::Point3f>& getCameraFloor() const
	{
		return m_camera_floor;
	}

	const cv::Point3f& getCameraLocation() const
	{
		return m_camera_location;
	}

	const std::vector<cv::Point3f>& getCameraPlane() const
	{
		return m_camera_plane;
	}
};

} /* namespace nl_uu_science_gmt */

#endif /* CAMERA_H_ */
