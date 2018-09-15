#include "SBVHBuilder.h"
#include <algorithm>
#include <functional>
#include <thread>


SBVHBuilder::SBVHBuilder(std::vector<triangle> geometry, unsigned int levels, float binLength){
	
	geom = geometry;

	binLen = std::max(0.00001f, std::min(1.0f, binLength));

	std::vector<unsigned int> g(geom.size());
	for (unsigned int i = 0; i < geom.size(); i++)
		g[i] = i;

	
	nodes.reserve(pow(2, levels));
	computeCenters();
	bvhConstruction(0, 0, g, levels, glm::vec4(), glm::vec4(), OBJECT);
	
}

std::vector<BoundingVolume> SBVHBuilder::buildBVH(){

	return nodes;
}

std::vector<unsigned int> SBVHBuilder::getIndices(){

	return indices;
}

void SBVHBuilder::bvhConstruction(unsigned int parent, unsigned int node, std::vector<unsigned int> geometry, unsigned levelCount, glm::vec4 lBorder, glm::vec4 rBorder, spliting spl, axis ax){

#define LEFT_CHILD 0
#define RIGHT_CHILD 1

	glm::vec4 minCoord = glm::vec4(), maxCoord = glm::vec4();
	
	//assert(minCoord.x <= maxCoord.x);
	//assert(minCoord.y <= maxCoord.y);
	//assert(minCoord.z <= maxCoord.z);
	
	if (geometry.size() == 0) {
		return;
	}
	

	// finding minimal AABB
	if (spl == SPATIAL) {

		minCoord = lBorder;
		maxCoord = rBorder;
	
	}
	else{
		findMinMax(geometry, minCoord, maxCoord);
		if (nodes.size() > 0) {
			minCoord.x = std::max(minCoord.x, nodes[parent].start.x);
			minCoord.y = std::max(minCoord.y, nodes[parent].start.y);
			minCoord.z = std::max(minCoord.z, nodes[parent].start.z);

			maxCoord.x = std::min(maxCoord.x, nodes[parent].end.x);
			maxCoord.y = std::min(maxCoord.y, nodes[parent].end.y);
			maxCoord.z = std::min(maxCoord.z, nodes[parent].end.z);

			//assert(minCoord.x >= nodes[parent].start.x);
			//assert(minCoord.y >= nodes[parent].start.y);
			//assert(minCoord.z >= nodes[parent].start.z);

			//assert(maxCoord.x <= nodes[parent].end.x);
			//assert(maxCoord.y <= nodes[parent].end.y);
			//assert(maxCoord.z <= nodes[parent].end.z);

			//assert(nodes[parent].start.x <= nodes[parent].end.x);
			//assert(nodes[parent].start.y <= nodes[parent].end.y);
			//assert(nodes[parent].start.z <= nodes[parent].end.z);

		}
		
	}

	AABB vol(minCoord, maxCoord);

	vol.parent = parent;
	vol.left = vol.right = 0;
	vol.indices_count = vol.start_index = 0;
	nodes.push_back(vol);

	node == LEFT_CHILD ? nodes[parent].left = nodes.size() - 1 : nodes[parent].right = nodes.size() - 1;
	
	//#define BV_D_SIZE
#ifdef BV_D_SIZE
	printf("BUILD %f %f %f %f %f %f\n", vol.start.x, vol.start.y, vol.start.z, vol.end.x, vol.end.y, vol.end.z);
#endif

	// recursion end - assign primitives
	if (levelCount == 0) {
		
		nodes[nodes.size() - 1].start_index = indices.size();
		nodes[nodes.size() - 1].indices_count = geometry.size();
		indices.insert(indices.end(), geometry.begin(), geometry.end());
		nodes[nodes.size() - 1].left = nodes[nodes.size() - 1].right = 0;

		return;
	}

	// compute SAH
	float splitLine;
	spliting sp = OBJECT;
	

	splitLine = evaluateSAH(geometry, ax, sp);

	unsigned int par = nodes.size() - 1;
	
	glm::vec4 lDivide, rDivide;
	if (ax == X_AXIS){
		//assert((minCoord.x <= splitLine) && (splitLine <= maxCoord.x));
		lDivide = glm::vec4(splitLine, minCoord.y, minCoord.z, minCoord.w);
		rDivide = glm::vec4(splitLine, maxCoord.y, maxCoord.z, maxCoord.w);
	}
	else if (ax == Y_AXIS){
		//assert((minCoord.y <= splitLine) && (splitLine <= maxCoord.y));
		lDivide = glm::vec4(minCoord.x, splitLine, minCoord.z, minCoord.w);
		rDivide = glm::vec4(maxCoord.x, splitLine, maxCoord.z, maxCoord.w);
	}
	else if (ax == Z_AXIS){
		//assert((minCoord.z <= splitLine) && (splitLine <= maxCoord.z));
		lDivide = glm::vec4(minCoord.x, minCoord.y, splitLine, minCoord.w);
		rDivide = glm::vec4(maxCoord.x, maxCoord.y, splitLine, maxCoord.w);
	}
	
	//assert(lDivide.x <= rDivide.x);
	//assert(lDivide.y <= rDivide.y);
	//assert(lDivide.z <= rDivide.z);

	// left child
	std::vector<unsigned int> gm;
	gm = trianglesLowerBound(geometry, splitLine, ax, SPATIAL);
	bvhConstruction(par, LEFT_CHILD, gm, levelCount - 1, minCoord, rDivide, sp, ax == X_AXIS ? Y_AXIS : ax == Y_AXIS ? Z_AXIS : X_AXIS);
	
	// right child
	std::vector<unsigned int> gt;
	gt = trianglesUpperBound(geometry, splitLine, ax, SPATIAL);
	bvhConstruction(par, RIGHT_CHILD, gt, levelCount - 1, lDivide, maxCoord, sp, ax == X_AXIS ? Y_AXIS : ax == Y_AXIS ? Z_AXIS : X_AXIS);

}


void SBVHBuilder::findMinMax(std::vector<unsigned int> geometry, glm::vec4 & minCoord, glm::vec4 & maxCoord){
	
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

unsigned int SBVHBuilder::trianglesUnder(std::vector<unsigned int> geometry, float criteria, axis ax){

	return trianglesLowerBound(geometry, criteria, ax).size();
}

float SBVHBuilder::trianglesUnderSpatial(std::vector<unsigned int> geometry, float criteria, float _min, axis ax){

	register float count = 0.0f;
	register glm::vec3 points = glm::vec3();

	for (auto it = geometry.begin(); it != geometry.end(); it++) {
		
		points = ax == X_AXIS ? glm::vec3(geom[*it].vertex_a.x, geom[*it].vertex_b.x, geom[*it].vertex_c.x) : ax == Y_AXIS ? glm::vec3(geom[*it].vertex_a.y, geom[*it].vertex_b.y, geom[*it].vertex_c.y) : glm::vec3(geom[*it].vertex_a.z, geom[*it].vertex_b.z, geom[*it].vertex_c.z);

		if (points.x >= _min && points.x <= criteria && points.y >= _min && points.y <= criteria && points.z >= _min && points.z <= criteria)
			count += 1.0f;
		else {
			float t = std::max(points.x, std::max(points.y, points.z)) - std::min(points.x, std::min(points.y, points.z));

			float n = std::max(_min, std::min(points.x, std::min(points.y, points.z)));
			float m = std::min(criteria, std::max(points.x, std::max(points.y, points.z)));

			if(m >= n){
				count += (m - n) / t;
				//assert((((m - n) / t) <= 1.0f) && (((m - n) / t) >= 0.0f));
			}
		}

	}
	return count;
}

std::vector<unsigned int> SBVHBuilder::trianglesLowerBound(std::vector<unsigned int> geometry, float criteria, axis ax, spliting split){

	std::vector<unsigned int> resultSet;
	resultSet.reserve(geometry.size());

	for (auto it = geometry.begin(); it != geometry.end(); it++) {
		
		if (split == OBJECT) {
			glm::vec4 centroid = centroids[*it];
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
		else {
			if (ax == X_AXIS) {
				if (geom[*it].vertex_a.x < criteria || geom[*it].vertex_b.x < criteria || geom[*it].vertex_c.x < criteria)
					resultSet.push_back(*it);
			}
			else if (ax == Y_AXIS) {
				if (geom[*it].vertex_a.y < criteria || geom[*it].vertex_b.y < criteria || geom[*it].vertex_c.y < criteria)
					resultSet.push_back(*it);
			}
			else if (ax == Z_AXIS) {
				if (geom[*it].vertex_a.z < criteria || geom[*it].vertex_b.z < criteria || geom[*it].vertex_c.z < criteria)
					resultSet.push_back(*it);
			}
		}
	}

	return resultSet;
}

std::vector<unsigned int> SBVHBuilder::trianglesUpperBound(std::vector<unsigned int> geometry, float criteria, axis ax, spliting split){

	std::vector<unsigned int> resultSet;
	resultSet.reserve(geometry.size());

	for (auto it = geometry.begin(); it != geometry.end(); it++) {
		
		if (split == OBJECT) {
			glm::vec4 centroid = centroids[*it];
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
		else {
			if (ax == X_AXIS) {
				if (geom[*it].vertex_a.x > criteria || geom[*it].vertex_b.x > criteria || geom[*it].vertex_c.x > criteria)
					resultSet.push_back(*it);
			}
			else if (ax == Y_AXIS) {
				if (geom[*it].vertex_a.y > criteria || geom[*it].vertex_b.y > criteria || geom[*it].vertex_c.y > criteria)
					resultSet.push_back(*it);
			}
			else if (ax == Z_AXIS) {
				if (geom[*it].vertex_a.z > criteria || geom[*it].vertex_b.z > criteria || geom[*it].vertex_c.z > criteria)
					resultSet.push_back(*it);
			}
		}
	}

	return resultSet;
}

float SBVHBuilder::evaluateSAH(std::vector<unsigned int> geometry, axis ax, spliting & split){

	float splitLine = 0.0f, sah_o, sah_s;
	unsigned int a, b;

	float s_a = evaluateObjectSAH(geometry, ax, sah_o);
	float s_b = evaluateSpatialSAH(geometry, ax, sah_s);
	splitLine = sah_o < sah_s ? s_a : s_b;
	split = sah_o < sah_s ? OBJECT : SPATIAL;

	return splitLine;
}

float SBVHBuilder::evaluateObjectSAH(std::vector<unsigned int> geometry, axis ax, float & sahValue){

	glm::vec4 minCoord = glm::vec4(), maxCoord = glm::vec4();

	minCoord = nodes[nodes.size() - 1].start;
	maxCoord = nodes[nodes.size() - 1].end;

	register float bestSAH = std::numeric_limits<float>::max();
	register float splitLine = 0.0f;
	register float split = 0.0f, add;

	if (ax == X_AXIS) {
		add = (maxCoord.x - minCoord.x) * binLen;
		split = minCoord.x + add;
	}
	else if (ax == Y_AXIS) {
		add = (maxCoord.y - minCoord.y) * binLen;
		split = minCoord.y + add;	
	}
	else if (ax == Z_AXIS) {
		add = (maxCoord.z - minCoord.z) * binLen;
		split = minCoord.z + add;	
	}

	// best SAH by binning
	for (float bin = binLen; bin < 1.0f; bin += binLen) {

		// probability of intersection part a/b
		float pa = bin;
		float pb = 1.0f - bin;

		unsigned int under = trianglesUnder(geometry, split, ax);

		// SAH metric
		float sah = pa * under + pb * (geometry.size() - under);

		if (sah < bestSAH)
			splitLine = split;
		
		bestSAH = std::min(sah, bestSAH);
		split += add;
	}

	sahValue = bestSAH;

	return splitLine;
}

float SBVHBuilder::evaluateSpatialSAH(std::vector<unsigned int> geometry, axis ax, float & sahValue){

	glm::vec4 minCoord = glm::vec4(), maxCoord = glm::vec4();

	minCoord = nodes[nodes.size() - 1].start;
	maxCoord = nodes[nodes.size() - 1].end;

	register float bestSAH = std::numeric_limits<float>::max();
	register float splitLine = 0.0f;
	register float split = 0.0f, add;
	register float _min = ax == X_AXIS ? minCoord.x : ax == Y_AXIS ? minCoord.y : minCoord.z;

	if (ax == X_AXIS) {
		add = (maxCoord.x - minCoord.x) * binLen;
		split = minCoord.x + add;
	}
	else if (ax == Y_AXIS) {
		add = (maxCoord.y - minCoord.y) * binLen;
		split = minCoord.y + add;
	}
	else if (ax == Z_AXIS) {
		add = (maxCoord.z - minCoord.z) * binLen;
		split = minCoord.z + add;
	}

	// best SAH by binning
	for (float bin = binLen; bin < 1.0f; bin += binLen) {

		// probability of intersection part a/b
		float pa = bin;
		float pb = 1.0f - bin;

		float under = trianglesUnderSpatial(geometry, split, _min, ax);

		// SAH metric
		float sah = pa * under + pb * (geometry.size() - under);

		if (sah < bestSAH)
			splitLine = split;
		
		bestSAH = std::min(sah, bestSAH);
		split += add;
	}

	sahValue = bestSAH;

	return splitLine;
}
//#ifdef BVH_DRAW
std::vector<glm::vec3> SBVHBuilder::bvhToDraw(unsigned int level){

	traverseToDraw(0, level);
	AABB* ab = (AABB*)&nodes[0];
	return bvhDrawData;
}

void SBVHBuilder::computeCenters(){

	centroids = std::vector<glm::vec4>(geom.size());
	unsigned int i = 0;
	for (auto it = geom.begin(); it != geom.end(); it++, i++) {
		glm::vec4 centroid = ((*it).vertex_a + (*it).vertex_b + (*it).vertex_c) / 3.0f;
		centroids[i] = centroid;
	}
}

void SBVHBuilder::traverseToDraw(unsigned int node, unsigned int currentLevel){

	if (currentLevel < 0)
		return;

	if (currentLevel == 0) {

		if ((nodes[node].start.x > nodes[node].end.x) || (nodes[node].start.y > nodes[node].end.y) || (nodes[node].start.z > nodes[node].end.z)) return;

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
		if (nodes[node].left != 0) traverseToDraw(nodes[node].left, currentLevel - 1);
		if (nodes[node].right != 0) traverseToDraw(nodes[node].right, currentLevel - 1);
	}

}
//#endif
