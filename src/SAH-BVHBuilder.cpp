#include "SAH-BVHBuilder.h"

SAH_BVHBuilder::SAH_BVHBuilder(std::vector<triangle> geometry, unsigned int levels, float binLength){

	geom = geometry;

	binLen = std::max(0.00001f, std::min(1.0f, binLength));

	std::vector<unsigned int> g;
	for (unsigned int i = 0; i < geom.size(); i++)
		g.push_back(i);

	bvhConstruction(0, 0, g, levels);

}

std::vector<BoundingVolume> SAH_BVHBuilder::buildBVH(){
	return nodes;
}

std::vector<unsigned int> SAH_BVHBuilder::getIndices(){
	return indices;
}

void SAH_BVHBuilder::bvhConstruction(unsigned int parent, unsigned int node, std::vector<unsigned int> geometry, unsigned levelCount, axis ax){

#define LEFT_CHILD 0
#define RIGHT_CHILD 1

	glm::vec4 minCoord = glm::vec4(), maxCoord = glm::vec4();

	// finding minimal AABB
	findMinMax(geometry, minCoord, maxCoord);
	AABB vol(minCoord, maxCoord);

	vol.parent = parent;
	vol.left = vol.right = 0;
	vol.indices_count = vol.start_index = 0;
	nodes.push_back(vol);

	node == LEFT_CHILD ? nodes[parent].left = nodes.size() - 1 : nodes[parent].right = nodes.size() - 1;

//#define BV_D_SIZE
#ifdef BV_D_SIZE
	printf("%f %f %f %f\n", vol.start.x, vol.start.y, vol.end.x, vol.end.y);
#endif

	if (geometry.size() == 0) {
		nodes[nodes.size() - 1].left = nodes[nodes.size() - 1].right = 0;
		//return;
	}

	// recursion end - assign primitives
	if (levelCount == 0) {
		//vol->primitives = geometry;
		//printf("leaf %d %d\n", geometry.size(), indices.size());
		//printf("%d\n", geometry[0]);
		nodes[nodes.size() - 1].start_index = indices.size();
		nodes[nodes.size() - 1].indices_count = geometry.size();
		indices.insert(indices.end(), geometry.begin(), geometry.end());
		nodes[nodes.size() - 1].left = nodes[nodes.size() - 1].right = 0;

		return;
	}


	// compute SAH
	float splitLine;

	splitLine = evaluateSAH(geometry, ax);
	unsigned int par = nodes.size() - 1;

	assert(minCoord.y <= maxCoord.y);

	// left child
	std::vector<unsigned int> gm;
	gm = trianglesLowerBound(geometry, splitLine, ax);
	bvhConstruction(par, LEFT_CHILD, gm, levelCount - 1, ax == X_AXIS ? Y_AXIS : ax == Y_AXIS ? Z_AXIS : X_AXIS);

	// right child
	std::vector<unsigned int> gt;
	gt = trianglesUpperBound(geometry, splitLine, ax);
	bvhConstruction(par, RIGHT_CHILD, gt, levelCount - 1, ax == X_AXIS ? Y_AXIS : ax == Y_AXIS ? Z_AXIS : X_AXIS);
}

void SAH_BVHBuilder::findMinMax(std::vector<unsigned int> geometry, glm::vec4 & minCoord, glm::vec4 & maxCoord){

	if (geometry.size() > 0) {
		minCoord = glm::vec4(geom[geometry[0]].vertex_a.x, geom[geometry[0]].vertex_a.y, geom[geometry[0]].vertex_a.z, 0.0);
		maxCoord = glm::vec4(geom[geometry[0]].vertex_a.x, geom[geometry[0]].vertex_a.y, geom[geometry[0]].vertex_a.z, 0.0);
	}

	for (auto it = geometry.begin(); it != geometry.end(); it++) {

		minCoord.x = std::min(minCoord.x, std::min(geom[*it].vertex_a.x, std::min(geom[*it].vertex_b.x, geom[*it].vertex_c.x)));
		minCoord.y = std::min(minCoord.y, std::min(geom[*it].vertex_a.y, std::min(geom[*it].vertex_b.y, geom[*it].vertex_c.y)));
		minCoord.z = std::min(minCoord.z, std::min(geom[*it].vertex_a.z, std::min(geom[*it].vertex_b.z, geom[*it].vertex_c.z)));

		maxCoord.x = std::max(maxCoord.x, std::max(geom[*it].vertex_a.x, std::max(geom[*it].vertex_b.x, geom[*it].vertex_c.x)));
		maxCoord.y = std::max(maxCoord.y, std::max(geom[*it].vertex_a.y, std::max(geom[*it].vertex_b.y, geom[*it].vertex_c.y)));
		maxCoord.z = std::max(maxCoord.z, std::max(geom[*it].vertex_a.z, std::max(geom[*it].vertex_b.z, geom[*it].vertex_c.z)));
	}

}

unsigned int SAH_BVHBuilder::trianglesUnder(std::vector<unsigned int> geometry, float criteria, axis ax){
	return trianglesLowerBound(geometry, criteria, ax).size();
}

std::vector<unsigned int> SAH_BVHBuilder::trianglesLowerBound(std::vector<unsigned int> geometry, float criteria, axis ax){

	std::vector<unsigned int> resultSet;

	for (auto it = geometry.begin(); it != geometry.end(); it++) {

		glm::vec4 centroid = (geom[*it].vertex_a + geom[*it].vertex_b + geom[*it].vertex_c) / 3.0f;

		if (ax == X_AXIS) {
			if (centroid.x < criteria)
				resultSet.push_back(*it);
		}
		else if (ax == Y_AXIS) {
			if (centroid.y < criteria)
				resultSet.push_back(*it);
		}
		else if (ax == Z_AXIS) {
			if (centroid.z < criteria)
				resultSet.push_back(*it);
		}
	}

	return resultSet;
}

std::vector<unsigned int> SAH_BVHBuilder::trianglesUpperBound(std::vector<unsigned int> geometry, float criteria, axis ax){

	std::vector<unsigned int> resultSet;

	for (auto it = geometry.begin(); it != geometry.end(); it++) {
		
		glm::vec4 centroid = (geom[*it].vertex_a + geom[*it].vertex_b + geom[*it].vertex_c) / 3.0f;

		if (ax == X_AXIS) {
			if (centroid.x >= criteria)
				resultSet.push_back(*it);
		}
		else if (ax == Y_AXIS) {
			if (centroid.y >= criteria)
				resultSet.push_back(*it);
		}
		else if (ax == Z_AXIS) {
			if (centroid.z >= criteria)
				resultSet.push_back(*it);
		}
		
	}

	return resultSet;
}

float SAH_BVHBuilder::evaluateSAH(std::vector<unsigned int> geometry, axis ax){

	glm::vec4 minCoord = glm::vec4(), maxCoord = glm::vec4();

	minCoord = nodes[nodes.size() - 1].start;
	maxCoord = nodes[nodes.size() - 1].end;

	float bestSAH = std::numeric_limits<float>::max();
	float splitLine = 0.0f;

	// best SAH by binning
	for (float bin = binLen; bin < 1.0f; bin += binLen) {

		float split = 0.0f;

		// splitting by axis
		if (ax == X_AXIS) {
			split = (maxCoord.x - minCoord.x) * bin;
			split += minCoord.x;
		}
		else if (ax == Y_AXIS) {
			split = (maxCoord.y - minCoord.y) * bin;
			split += minCoord.y;
		}
		else if (ax == Z_AXIS) {
			split = (maxCoord.z - minCoord.z) * bin;
			split += minCoord.z;
		}


		// probability of intersection part a/b
		float pa = bin;
		float pb = 1.0f - bin;

		unsigned int under = trianglesUnder(geometry, split, ax);

		// SAH metric
		float sah = pa * under + pb * (geometry.size() - under);

		if (sah < bestSAH)
			splitLine = split;

		bestSAH = std::min(sah, bestSAH);
	}

	return splitLine;
}

std::vector<glm::vec3> SAH_BVHBuilder::bvhToDraw(unsigned int level){
	
	traverseToDraw(0, level);
	
	return bvhDrawData;
}

void SAH_BVHBuilder::traverseToDraw(unsigned int node, unsigned int currentLevel){

	if (currentLevel < 0)
		return;

	if (currentLevel == 0) {

		BoundingVolume bv = nodes[node];

		bvhDrawData.push_back(glm::vec3(bv.start));
		bvhDrawData.push_back(glm::vec3(bv.start.x, bv.end.y, bv.start.z));

		bvhDrawData.push_back(glm::vec3(bv.start));
		bvhDrawData.push_back(glm::vec3(bv.end.x, bv.start.y, bv.start.z));

		bvhDrawData.push_back(glm::vec3(bv.start.x, bv.end.y, bv.start.z));
		bvhDrawData.push_back(glm::vec3(bv.end.x, bv.end.y, bv.start.z));

		bvhDrawData.push_back(glm::vec3(bv.end.x, bv.start.y, bv.start.z));
		bvhDrawData.push_back(glm::vec3(bv.end.x, bv.end.y, bv.start.z));

		bvhDrawData.push_back(glm::vec3(bv.start));
		bvhDrawData.push_back(glm::vec3(bv.start.x, bv.start.y, bv.end.z));

		bvhDrawData.push_back(glm::vec3(bv.end.x, bv.end.y, bv.start.z));
		bvhDrawData.push_back(glm::vec3(bv.end));

		bvhDrawData.push_back(glm::vec3(bv.start.x, bv.end.y, bv.start.z));
		bvhDrawData.push_back(glm::vec3(bv.start.x, bv.end.y, bv.end.z));

		bvhDrawData.push_back(glm::vec3(bv.end.x, bv.start.y, bv.start.z));
		bvhDrawData.push_back(glm::vec3(bv.end.x, bv.start.y, bv.end.z));

		bvhDrawData.push_back(glm::vec3(bv.start.x, bv.start.y, bv.end.z));
		bvhDrawData.push_back(glm::vec3(bv.start.x, bv.end.y, bv.end.z));

		bvhDrawData.push_back(glm::vec3(bv.start.x, bv.start.y, bv.end.z));
		bvhDrawData.push_back(glm::vec3(bv.end.x, bv.start.y, bv.end.z));

		bvhDrawData.push_back(glm::vec3(bv.start.x, bv.end.y, bv.end.z));
		bvhDrawData.push_back(glm::vec3(bv.end));

		bvhDrawData.push_back(glm::vec3(bv.end.x, bv.start.y, bv.end.z));
		bvhDrawData.push_back(glm::vec3(bv.end));



	}
	else {
		traverseToDraw(nodes[node].left, currentLevel - 1);
		traverseToDraw(nodes[node].right, currentLevel - 1);
	}

}
