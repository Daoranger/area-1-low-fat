
//--------------------------------------------------------------
//
//  Kevin M. Smith
//
//  Simple Octree Implementation 11/10/2020
// 
//  Copyright (c) by Kevin M. Smith
//  Copying or use without permission is prohibited by law.
//
#pragma once
#include "ofMain.h"
#include "box.h"
#include "ray.h"


// Each node have a "box", a "list of points", and a "list of children"
//
class TreeNode {														// Each node in octree = a cube of space
public:
	Box box;															// The 3D bounding box of this node
	vector<int> points;													// A list of indices of points (vertices) from the mesh that lie inside this box
	vector<TreeNode> children;											// The 8 smaller boxes (nodes) that subdivided from this one
};


class Octree {															// The main Octree structure that holds the mesh and root node
public:
	
	void create(const ofMesh & mesh, int numLevels);									// Builds the entire octree from a mesh up to a specified number of levels
	void subdivide(const ofMesh & mesh, TreeNode & node, int numLevels, int level);		// Recursively subdivides a node into 8 smaller boxes until the desired depth (once numLevels is reached) or until each node contains only one point
	bool intersect(const Ray &, const TreeNode & node, TreeNode & nodeRtn);				// Checks whether a ray (from mouse click) intersects with any boxes in the tree.
	bool intersect(const Box &, TreeNode & node, vector<Box> & boxListRtn, vector<TreeNode> & nodeListRtn);				// Check if a given box (like the moon lander's boudning box) overlaps any boxes in the octree. Stores all intersecting boxes in boxListRtn
	void draw(TreeNode & node, int numLevels, int level);								// Draws the octree recursively, showing the boxes at each level (used to visualize how the space is subdivided)
	void draw(int numLevels, int level) {												// Helper function that starts drawing from the root node (just call the recursive "draw" version above)
		draw(root, numLevels, level);
	}
	void drawLeafNodes(TreeNode & node);												// Draws ony the leaf nodes (the smallest cubes that contain actual points)
	static void drawBox(const Box &box);												// Function to draw a single box on screen
	static Box meshBounds(const ofMesh &);
	int getMeshPointsInBox(const ofMesh &mesh, const vector<int> & points, Box & box, vector<int> & pointsRtn);		// Finds which points from the mesh lie inside a given box. Stores them in pointsRtn
	int getMeshFacesInBox(const ofMesh &mesh, const vector<int> & faces, Box & box, vector<int> & facesRtn);		// Finds which faces (triangles) from the mesh lie inside a given box.
	void subDivideBox8(const Box &b, vector<Box> & boxList);							// Splits on big box into 8 smaller boxes, one for each octant

	ofMesh mesh;						// The 3D model (terrain) the tree is built on
	TreeNode root;						// The root (top-level) node of the octree (the entire terrain's/model boudning box)
	bool bUseFaces = false;				// Tells whether if we building the tree base don faces (triangle) or just points

	// debug;
	//
	int strayVerts= 0;					// Counts points that didn't fit into any box (should be 0 ideally)
	int numLeaf = 0;					// Counts how many leaf nodes exist (useful for checking correctness)

	vector<ofColor> levelColors = { ofColor::red, ofColor::orange, ofColor::yellow, ofColor::green, ofColor::cyan, ofColor::blue, ofColor::magenta, ofColor::white };

};