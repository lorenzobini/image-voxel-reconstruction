/*
 * VoxelReconstruction.cpp
 *
 *  Created on: Nov 13, 2013
 *      Author: coert
 */

#include "VoxelReconstruction.h"

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/highgui/highgui_c.h>
#include <stddef.h>
#include <cassert>
#include <iostream>
#include <sstream>

#include "controllers/Glut.h"
#include "controllers/Reconstructor.h"
#include "controllers/Scene3DRenderer.h"
#include "utilities/General.h"

using namespace nl_uu_science_gmt;
using namespace std;
using namespace cv;

namespace nl_uu_science_gmt
{

/**
 * Main constructor, initialized all cameras
 */
VoxelReconstruction::VoxelReconstruction(const string &dp, const int cva) :
		m_data_path(dp), m_cam_views_amount(cva)
{
	const string cam_path = m_data_path + "cam";

	for (int v = 0; v < m_cam_views_amount; ++v)
	{
		stringstream full_path;
		full_path << cam_path << (v + 1) << PATH_SEP;

		/*
		 * Assert that there's a background image or video file and \
		 * that there's a video file
		 */
		std::cout << full_path.str() << General::BackgroundImageFile << std::endl;
		std::cout << full_path.str() << General::VideoFile << std::endl;
		assert(
			General::fexists(full_path.str() + General::BackgroundImageFile)
			&&
			General::fexists(full_path.str() + General::VideoFile)
		);

		/*
		 * Assert that if there's no config.xml file, there's an intrinsics file and
		 * a checkerboard video to create the extrinsics from
		 */
		assert(
			(!General::fexists(full_path.str() + General::ConfigFile) ?
				General::fexists(full_path.str() + General::IntrinsicsFile) &&
					General::fexists(full_path.str() + General::CheckerboadVideo)
			 : true)
		);

		m_cam_views.push_back(new Camera(full_path.str(), General::ConfigFile, v));
	}
}

/**
 * Main destructor, cleans up pointer vector memory of the cameras
 */
VoxelReconstruction::~VoxelReconstruction()
{
	for (size_t v = 0; v < m_cam_views.size(); ++v)
		delete m_cam_views[v];
}

/**
 * What you can hit
 */
void VoxelReconstruction::showKeys()
{
	cout << "VoxelReconstruction v" << VERSION << endl << endl;
	cout << "Use these keys:" << endl;
	cout << "q       : Quit" << endl;
	cout << "p       : Pause" << endl;
	cout << "b       : Frame back" << endl;
	cout << "n       : Next frame" << endl;
	cout << "r       : Rotate voxel space" << endl;
	cout << "s       : Show/hide arcball wire sphere (Linux only)" << endl;
	cout << "v       : Show/hide voxel space box" << endl;
	cout << "g       : Show/hide ground plane" << endl;
	cout << "c       : Show/hide cameras" << endl;
	cout << "i       : Show/hide camera numbers (Linux only)" << endl;
	cout << "o       : Show/hide origin" << endl;
	cout << "t       : Top view" << endl;
	cout << "1,2,3,4 : Switch camera #" << endl << endl;
	cout << "Zoom with the scrollwheel while on the 3D scene" << endl;
	cout << "Rotate the 3D scene with left click+drag" << endl << endl;
}

/**
 * - If the xml-file with camera intrinsics, extrinsics and distortion is missing,
 *   create it from the checkerboard video and the measured camera intrinsics
 * - After that initialize the scene rendering classes
 * - Run it!
 */
void VoxelReconstruction::run(int argc, char** argv)
{
	for (int v = 0; v < m_cam_views_amount; ++v)
	{
		bool has_cam = Camera::detExtrinsics(m_cam_views[v]->getDataPath(), General::CheckerboadVideo,
				General::IntrinsicsFile, m_cam_views[v]->getCamPropertiesFile());
		assert(has_cam);
		if (has_cam) has_cam = m_cam_views[v]->initialize();
		assert(has_cam);
	}

	destroyAllWindows();
	namedWindow(VIDEO_WINDOW, CV_WINDOW_KEEPRATIO);

	Reconstructor reconstructor(m_cam_views);
	Scene3DRenderer scene3d(reconstructor, m_cam_views);
	Glut glut(scene3d);

#ifdef __linux__
	glut.initializeLinux(SCENE_WINDOW.c_str(), argc, argv);
#elif defined _WIN32
	glut.initializeWindows(SCENE_WINDOW.c_str());
	glut.mainLoopWindows();
#endif
}

} /* namespace nl_uu_science_gmt */
