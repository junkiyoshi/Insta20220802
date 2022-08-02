#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(30);
	ofSetWindowTitle("openFrameworks");

	ofBackground(255);
	ofSetLineWidth(1.5);
	ofEnableDepthTest();

	this->frame.setMode(ofPrimitiveMode::OF_PRIMITIVE_LINES);
}

//--------------------------------------------------------------
void ofApp::update() {

	ofSeedRandom(39);

	this->face.clear();
	this->frame.clear();

	int span = 10;
	float threshold = 0.5;
	float frame_param = ofGetFrameNum() * 0.05;
	float coordinate_param = 0.0065;
	int len = 300;
	ofColor line_color(255);

	int z = len + span * 0.5;

	float angle;
	glm::highp_mat4 rotation, twist;
	for (int i = 0; i <= 6; i++) {

		for (int x = -len; x <= len; x += span) {

			for (int y = -len; y <= len; y += span) {

				if (i <= 4) {

					angle = PI * 0.5 * i;
					rotation = glm::rotate(glm::mat4(), angle, glm::vec3(0, 1, 0));
				}

				if (i == 5) {

					angle = PI * 0.5;
					rotation = glm::rotate(glm::mat4(), angle, glm::vec3(1, 0, 0));
				}

				if (i == 6) {

					angle = PI * 0.5 * 3;
					rotation = glm::rotate(glm::mat4(), angle, glm::vec3(1, 0, 0));
				}

				auto index = this->face.getNumVertices();
				vector<glm::vec3> vertices;

				vertices.push_back(glm::vec4(x - span * 0.5, y - span * 0.5, z, 0) * rotation);
				vertices.push_back(glm::vec4(x + span * 0.5, y - span * 0.5, z, 0) * rotation);
				vertices.push_back(glm::vec4(x + span * 0.5, y + span * 0.5, z, 0) * rotation);
				vertices.push_back(glm::vec4(x - span * 0.5, y + span * 0.5, z, 0) * rotation);

				for (auto& vertex : vertices) {

					auto noise_value = ofNoise(vertex.y * 0.0005 + ofGetFrameNum() * 0.02);
					if (noise_value < 0.43)	{

						noise_value = ofMap(noise_value, 0, 0.43, -1, 0);
					}
					else if (noise_value > 0.57) { 

						noise_value = ofMap(noise_value, 0.57, 1, 0, 1);
					}
					else {

						noise_value = 0;
					}
					auto twist_rad = ofMap(noise_value, -1, 1, PI * -4, PI * 4);
					twist = glm::rotate(glm::mat4(), twist_rad, glm::vec3(0, 1, 0));


					vertex = glm::vec4(vertex, 0) * twist;
				}

				this->face.addVertices(vertices);

				this->face.addIndex(index + 0); this->face.addIndex(index + 1); this->face.addIndex(index + 2);
				this->face.addIndex(index + 0); this->face.addIndex(index + 2); this->face.addIndex(index + 3);

				if (x == -len) {

					this->frame.addVertex(vertices[0]);
					this->frame.addVertex(vertices[3]);

					this->frame.addColor(line_color);
					this->frame.addColor(line_color);

					this->frame.addIndex(this->frame.getNumVertices() - 1);
					this->frame.addIndex(this->frame.getNumVertices() - 2);
				}

				if (y == len) {

					this->frame.addVertex(vertices[2]);
					this->frame.addVertex(vertices[3]);

					this->frame.addColor(line_color);
					this->frame.addColor(line_color);

					this->frame.addIndex(this->frame.getNumVertices() - 1);
					this->frame.addIndex(this->frame.getNumVertices() - 2);
				}

				if (x == len) {

					this->frame.addVertex(vertices[1]);
					this->frame.addVertex(vertices[2]);

					this->frame.addColor(line_color);
					this->frame.addColor(line_color);

					this->frame.addIndex(this->frame.getNumVertices() - 1);
					this->frame.addIndex(this->frame.getNumVertices() - 2);
				}

				if (y == -len) {

					this->frame.addVertex(vertices[0]);
					this->frame.addVertex(vertices[1]);

					this->frame.addColor(line_color);
					this->frame.addColor(line_color);

					this->frame.addIndex(this->frame.getNumVertices() - 1);
					this->frame.addIndex(this->frame.getNumVertices() - 2);

				}
			}
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->cam.begin();
	//ofRotateY(ofGetFrameNum());

	this->frame.draw();

	ofSetColor(0);
	this->face.draw();

	this->cam.end();
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}