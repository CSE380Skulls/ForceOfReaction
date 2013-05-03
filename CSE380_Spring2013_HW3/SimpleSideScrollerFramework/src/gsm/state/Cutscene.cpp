#include "stdafx.h"
#include "src/gui/OverlayImage.h"
#include "Cutscene.h"

Cutscene::Cutscene(){
	numImages=0;
	currentImage=0;
};
Cutscene::~Cutscene(){
	overlayImages.clear();
	delete &overlayImages;
};

void Cutscene::addImage(OverlayImage* image){
	overlayImages.push_back(image);
	numImages++;
}

OverlayImage* Cutscene::nextImage(){
	return overlayImages.at(currentImage++);
}