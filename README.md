# objectrekt

ObjectRekt uses the FLIR Lepton, a longwave infrared thermal imager, along with OpenCV object recognition to create an automated camera that observes the scene and tracks a presenter's location, panning to the proper locations.

Behind the camera is a C++ program that analyzes the environment and initializes the static location of a "central" point (such as a whiteboard or a projection) using an object recognition technique involving color thresholding. Then, using the Lepton Python SDK, the system recognizes the presenter's location based on the body's thermal signature and determines the best path for the camera to take in order to optimize the presentation.

###Goal

Our program aims to replace the cameraman in a presentation environment. By automating camera movements, we can provide an easier means to record and share a smooth and interactive video stream. Intended gestures could even be interpreted in order to produce smarter camera movement (for example, pointing to the board causes camera to pan to the board itself). Our goal is to have ObjectRekt be used for a wide variety of presentations such as lectures, shows, and demonstrations.