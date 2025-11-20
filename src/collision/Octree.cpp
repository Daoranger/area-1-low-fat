
//--------------------------------------------------------------
//
//  Kevin M. Smith
//
//  Simple Octree Implementation 11/10/2020
// 
//  Copyright (c) by Kevin M. Smith
//  Copying or use without permission is prohibited by law. 
//


#include "Octree.h"
 


//draw a box from a "Box" class  
//
void Octree::drawBox(const Box &box) {
	Vector3 min = box.parameters[0];							// Read min corner 
	Vector3 max = box.parameters[1];							// Read max corner
	Vector3 size = max - min;									// Compute the edge lengths in each axis
	Vector3 center = size / 2 + min;							// Compute box center 
	ofVec3f p = ofVec3f(center.x(), center.y(), center.z());	// Convert to ofVec3f for OF draw calls
	float w = size.x();											// Width along x axis
	float h = size.y();											// Heigth along y axis
	float d = size.z();											// Depth along z axis
	ofDrawBox(p, w, h, d);										// Draw an axis-aligned box at center with given dimensions
}

// return a Mesh Bounding Box for the entire Mesh
// Compute AABB that encloses all vertices
//
Box Octree::meshBounds(const ofMesh & mesh) {
	int n = mesh.getNumVertices();								// Total number of vertices in the mesh
	ofVec3f v = mesh.getVertex(0);								// Use the first vertex to init min/max
	ofVec3f max = v;											// Start max at first vertex
	ofVec3f min = v;											// Start min at first vertext
	for (int i = 1; i < n; i++) {								// Loop over the remaining vertices
		
		// Update the x, y, and z max/min
		// and construct Box using the max/min
		//
		ofVec3f v = mesh.getVertex(i);
		
		if (v.x > max.x) max.x = v.x;
		else if (v.x < min.x) min.x = v.x;

		if (v.y > max.y) max.y = v.y;
		else if (v.y < min.y) min.y = v.y;

		if (v.z > max.z) max.z = v.z;
		else if (v.z < min.z) min.z = v.z;
	}
	cout << "vertices: " << n << endl;
//	cout << "min: " << min << "max: " << max << endl;
	return Box(Vector3(min.x, min.y, min.z), Vector3(max.x, max.y, max.z));
}

// getMeshPointsInBox:  return an array of indices to points in mesh that are contained 
//                      inside the Box.  Return count of points found;
//
int Octree::getMeshPointsInBox(const ofMesh & mesh, const vector<int>& points,
	Box & box, vector<int> & pointsRtn)
{
	int count = 0;

	// Iterate over candidate indices. Get each vertex (point) and check if that vertiex
	// lines inside box. If it is insdie the box, increment the counter and keep the mesh
	// index for that vertex.
	//
	for (int i = 0; i < points.size(); i++) {
		ofVec3f v = mesh.getVertex(points[i]);
		if (box.inside(Vector3(v.x, v.y, v.z))) {
			count++;
			pointsRtn.push_back(points[i]);
		}
	}

	// Return how many vertices (points) was inside the box
	//
	return count;
}

// getMeshFacesInBox: return an array of indices to Faces in mesh that are contained 
//                    inside the Box.  Return count of faces found;
//
int Octree::getMeshFacesInBox(const ofMesh & mesh, const vector<int>& faces,
	Box & box, vector<int> & facesRtn)
{
	int count = 0;
	// Similar to the function above but is using to check faces (triangles) instead of points
	// We use when the octree works with polygons instead of individual vertices
	//
	for (int i = 0; i < faces.size(); i++) {
		ofMeshFace face = mesh.getFace(faces[i]);
		ofVec3f v[3];
		v[0] = face.getVertex(0);
		v[1] = face.getVertex(1);
		v[2] = face.getVertex(2);
		Vector3 p[3];
		p[0] = Vector3(v[0].x, v[0].y, v[0].z);
		p[1] = Vector3(v[1].x, v[1].y, v[1].z);
		p[2] = Vector3(v[2].x, v[2].y, v[2].z);
		if (box.inside(p,3)) {
			count++;
			facesRtn.push_back(faces[i]);
		}
	}
	return count;
}

//  Subdivide a Box into eight(8) equal size boxes, return them in boxList;
//	Splits one big box into 8 smaller boxes of equal size
//
void Octree::subDivideBox8(const Box& box, vector<Box>& boxList) {
	Vector3 min = box.parameters[0];
	Vector3 max = box.parameters[1];
	Vector3 size = max - min;
	Vector3 center = size / 2 + min;
	float xdist = (max.x() - min.x()) / 2;
	float ydist = (max.y() - min.y()) / 2;
	float zdist = (max.z() - min.z()) / 2;
	Vector3 h = Vector3(0, ydist, 0);

	//  generate ground floor
	//
	Box b[8];
	b[0] = Box(min, center);
	b[1] = Box(b[0].min() + Vector3(xdist, 0, 0), b[0].max() + Vector3(xdist, 0, 0));
	b[2] = Box(b[1].min() + Vector3(0, 0, zdist), b[1].max() + Vector3(0, 0, zdist));
	b[3] = Box(b[2].min() + Vector3(-xdist, 0, 0), b[2].max() + Vector3(-xdist, 0, 0));

	boxList.clear();
	for (int i = 0; i < 4; i++)
		boxList.push_back(b[i]);

	// generate second story
	//
	for (int i = 4; i < 8; i++) {
		b[i] = Box(b[i - 4].min() + h, b[i - 4].max() + h);
		boxList.push_back(b[i]);
	}
}

// Start building the octree by:
// Finds the mesh's boudning box
// Stores all points in the root node
// Call subdivde() to recursively building child boxes
//
void Octree::create(const ofMesh & geo, int numLevels) {
	// initialize octree structure
	//

	mesh = geo;
	int level = 0;
	root.box = meshBounds(mesh);
	if (!bUseFaces) {
		for (int i = 0; i < mesh.getNumVertices(); i++) {
			root.points.push_back(i);
		}
	}
	else {
		// need to load face vertices here
		//
	}

	// recursively buid octree
	//
	level++;
    subdivide(mesh, root, numLevels, level);
}


//
// subdivide:  recursive function to perform octree subdivision on a mesh
//
//  subdivide(node) algorithm:
//     1) subdivide box in node into 8 equal side boxes - see helper function subDivideBox8().
//     2) For each child box
//            sort point data into each box  (see helper function getMeshFacesInBox())
//        if a child box contains at list 1 point
//            add child to tree
//            if child is not a leaf node (contains more than 1 point)
//               recursively call subdivide(child)
// Recusrively splits a node into 8 smllar box. Distributes points into them until the tree reaches
// the desired depth or each node has few points (a leaf)
//      
             
void Octree::subdivide(const ofMesh & mesh, TreeNode & node, int numLevels, int level) {
	if (level >= numLevels) return;
	

	// subdvide algorithm implemented here

	vector<Box> childBoxes;
	subDivideBox8(node.box, childBoxes);

	for (auto& cb : childBoxes)
	{
		vector<int> ptsInChild;
		int count = 0;

		
		count = getMeshPointsInBox(mesh, node.points, cb, ptsInChild);

		// If the child node is not a leaft node (have more than 1 point), add it 
		// the list of children (so we can keep track and continue to recursively 
		// subdivide them until we reach leaft node/ # of levels)
		//
		if (count > 1)
		{
			TreeNode child;
			child.box = cb;
			child.points = std::move(ptsInChild);			// Use std::move to avoid copy list of points
			node.children.push_back(std::move(child));		// Use std::move semantic to avoid copy list of children TreeNode
		}
	}

	// Now do the same thing (subdvidie) for each child nodes (that is not a leaf node)
	// We want to recursively do this until the child nodes only have 1 point or we have
	// reaches the max level of the tree.
	//
	for (auto& child : node.children)
	{
		if (child.points.size() > 1)
		{
			// Increment the level
			subdivide(mesh, child, numLevels, level + 1);
		}
	}

}

// Implement functions below for Homework project
//


// Test if a ray (from mouse click) intersects any box in the octree (use for selecting points in 3D space)
//
bool Octree::intersect(const Ray &ray, const TreeNode & node, TreeNode & nodeRtn) {

	float t0 = 0.0;
	float t1 = 10000.0;
	if (!node.box.intersect(ray, t0, t1))
	{
		return false;
	}

	// When reached leaf node, return true and return the leaf node
	if (node.children.empty())
	{
		nodeRtn = node;
		return true;
	}


	// Recurse into children
	for (auto& child : node.children)
	{
		if (intersect(ray, child, nodeRtn))
		{
			// Return true for the first leaf node, propagate hit success upward
			return true;
		}
	}

	return false;	// No hit in any child node
}

// Test if a given box (like moon lander's bouding box) overlaps with any octree boxes (used for collision detection)
// box is the reference to the box of the spacecraft
// node is the node of the terrain
// boxListRtn is the list of leaf nodes that intersect with 
// the bouding box of spacecraft
//
bool Octree::intersect(const Box &box, TreeNode & node, vector<Box> & boxListRtn, vector<TreeNode> & nodeListRtn) {
	// skip if no overlap
	if (!box.overlap(node.box)) return false;

	// leaf node overlap, add it to the box list to return
	if (node.children.empty())
	{
		boxListRtn.push_back(node.box);
		nodeListRtn.push_back(node);
		return true;
	}

	// recurse into children
	bool hit = false;
	for (auto& child : node.children)
	{
		if (intersect(box, child, boxListRtn, nodeListRtn))
		{
			hit = true;		// mark this child is hit (don't return yet)
		}
	}
	return hit;	
}
	

//	Draws the octree boxes recursively up to a certain level (help visulize how the space is divded)
//
void Octree::draw(TreeNode & node, int numLevels, int level) {
	
	if (level > numLevels) return;

	// Draw box and set color for each level (node)
	int colorIndex = (level - 1) % levelColors.size();
	ofSetColor(levelColors[colorIndex]);
	drawBox(node.box); 
	
	// Draw children node's boxes of the current node (one level deeper)
	for (auto& child : node.children) {
		draw(child, numLevels, level + 1);
	}
}

// Optional: Draws only the leaf nodes (smallest boxes containing points), useuful for debug the final tree structure
//
void Octree::drawLeafNodes(TreeNode & node) {

	// Leaf nodes don't have any children. So to draw leaf nodes only,
	// we will have to traverse through the tree until we reach the leaf nodes
	// Once we reached the leaf nodes, draw the box of that leaf node and return.

	if (node.children.empty()) {
		drawBox(node.box);
		return;
	}

	for (auto& child : node.children) {
		drawLeafNodes(child);
	}
}

