#include "Cloud.h"

Cloud::Cloud() {}

void Cloud::Generate(int branchNum, float initialRadius, float radiusDecay, int numOfIterations) {
	
	std::uniform_real_distribution<double> distribution(0.0, 1.0);

	nodes.push_back(CloudNode{ glm::vec3(0.0f), initialRadius, 0 });
	int curStart = 0, curEnd = 1;

	for (int iteration = 1; iteration < numOfIterations; iteration++) {

		for (int i = curStart; i < curEnd; i++) {

		}

		// Next iteration, expand upon these nodes
		curStart = curEnd;
		curEnd = nodes.size();
	}

}