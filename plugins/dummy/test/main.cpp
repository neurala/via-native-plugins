/*
 * Copyright Neurala Inc. 2013-2021. All rights reserved.
 *
 * Except as expressly permitted in the accompanying License Agreement, if at all, (a) you shall
 * not license, sell, rent, lease, transfer, assign, distribute, display, host, outsource, disclose
 * or otherwise commercially exploit or make this source code available to any third party; (b) you
 * shall not modify, make derivative works of, disassemble, reverse compile or reverse engineer any
 * part of the SDK; (c) You shall not access the SDK in order to build a similar or competitive
 * product or service; (d) no part of the this source may be copied, reproduced, distributed,
 * republished, downloaded, displayed, posted or transmitted in any form or by any means, including
 * but not limited to electronic, mechanical, photocopying, recording or other means; and (e) any
 * future release, update, or other addition to functionality of the SDK shall be subject to the
 * terms of the accompanying License Agreement. You must reproduce, on all copies made by you or
 * for you, and must not remove, alter, or obscure in any way all proprietary rights notices
 * (including copyright notices) of Neurala Inc or its suppliers on or within the copies of the
 * SDK. Any sample code provided with the SDK and designated as such are for illustrative purposes
 * only and are not to be included in your applications.
 *
 * In cases when the accompanying License Agreement permits redistribution of this file, the above
 * notice shall be reproduced its entirety in every copy of a distributed version of this file.
 */

#include <numeric>
#include <iostream>

#include "dummy.h"

int
main()
{
	using namespace neurala;
	CameraDiscovererDummy dummyDiscoverer;

	// Discover the cameras exposed by this plugin
	const auto connectedCameras = dummyDiscoverer();
	CameraInfo dummyCameraInfo = connectedCameras[0];
	DummyVideoSource dummyVideoSource(dummyCameraInfo);

	// Retrieve dummy metadata
	const ImageMetadata metadata = dummyVideoSource.metadata();
	std::cout << "Source metadata: " << metadata << std::endl;

	// Call dummy nextFrame
	if (!dummyVideoSource.nextFrame())
	{
		// Retrieve and print the image view's metadata
		const ImageView view = dummyVideoSource.frame();
		std::cout << "Image view metadata: " << view.metadata() << std::endl;
	}
}
