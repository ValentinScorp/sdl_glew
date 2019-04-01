#include "AnimateModel.h"

glm::mat4 AnimateModel::Bone::GetLocalToWorldMatrix()
{
	glm::mat4 FinalMat = glm::mat4(1.0f);
	glm::mat4 MatTemp = glm::mat4(1.0f);
	glm::mat4 MatRot = glm::mat4(1.0f);

    FinalMat = glm::translate(FinalMat, glm::fvec3(position.x, position.y, position.z));
    
    MatTemp = glm::rotate(MatTemp, rotation.x, glm::fvec3(1.0f, 0.0f, 0.0f));
    MatRot =  MatTemp * MatRot;
    MatTemp = glm::mat4(1.0);
    MatTemp = glm::rotate(MatTemp, rotation.y, glm::fvec3(0.0f, 1.0f, 0.0f));
    MatRot =  MatTemp * MatRot;
    MatTemp = glm::mat4(1.0);
    MatTemp = glm::rotate(MatTemp, rotation.z, glm::fvec3(0.0f, 0.0f, 1.0f));
    MatRot =  MatTemp * MatRot;
    
	FinalMat = FinalMat * MatRot;

	return FinalMat;
}


AnimateModel::AnimateModel()
{
	currentAnimation = nullptr;
	animCounter = 0;
	currentFrame = 0;
}


AnimateModel::~AnimateModel()
{
	weights.clear();

	for (auto a : animations)
		if (a != nullptr)
			delete a;

	for (auto b : bones)
		if (b != nullptr)
			delete b;
}

void AnimateModel::AddBone(glm::fvec3 p, glm::fvec3 r)
{
	auto b = new Bone(p, r);
	bones.push_back(b);
}

AnimateModel::Bone * AnimateModel::GetBone(size_t id)
{
	if (id < bones.size())
		return bones[id];
	return nullptr;
}

size_t AnimateModel::GetBonesNum()
{
	return bones.size();
}

void AnimateModel::AddVertexWeights(std::vector<Weight> w)
{
	weights.push_back(w);
}

void AnimateModel::AddAnimation(Animation * anim)
{
	animations.push_back(anim);
}

void AnimateModel::addVertex(glm::fvec3 pos, glm::fvec3 nor, glm::fvec2 uv)
{
	SmaVertex v;
	v.pos = pos;
	v.nor = nor;
	v.tex = uv;
	vertexes.push_back(v);

	vertexes.push_back(SmaVertex(pos, nor, uv));
}

void AnimateModel::addVertex(SmaVertex ver)
{
	vertexes.push_back(ver);
}

void AnimateModel::UpdateAnimation(float dt, std::vector<SmaVertex> &smaVerts)
{
	if (currentAnimation == nullptr) {
		return;
	}
	std::vector<Bone> bones;

	KeyFrame *kf = currentAnimation->GetKeyframe(currentFrame);

	animCounter += 1;
	if (animCounter >= 1) {
		animCounter = 0;
		currentFrame++;
	}
	if (currentFrame >= currentAnimation->GetKeyframesNum()) {
		currentFrame = 0;
	}    
	for (int i = 0; i < vertexes.size(); i++) {
		glm::fvec3 finalVecPositin(0.0f, 0.0f, 0.0f);

		for (int j = 0; j < weights[i].size(); j++) {
			Bone* init_bone = weights[i][j].bone;
			float weight = weights[i][j].weight;

			if (init_bone != nullptr) {
				Bone deform_bone(*(kf->GetPosition(init_bone)), *(kf->GetRotation(init_bone)));
				glm::mat4 bonemat = glm::inverse(init_bone->GetLocalToWorldMatrix());
				glm::mat4 deformbonemat = deform_bone.GetLocalToWorldMatrix();

				glm::fvec3 vertPos(smaVerts[i].pos);
				vertPos = bonemat * glm::vec4(vertPos, 1.0f);
                vertPos = deformbonemat * glm::vec4(vertPos, 1.0f);
				vertPos *= weight;

				finalVecPositin += vertPos;
			}
		}

		vertexes[i].pos.x = finalVecPositin.x;
		vertexes[i].pos.y = finalVecPositin.y;
		vertexes[i].pos.z = finalVecPositin.z;
	}
}

void AnimateModel::BeginAnimation(std::string aname)
{
	for (auto a : animations) {
		if (a->GetName() == aname) {
			currentAnimation = a;
		}
	}
}

void AnimateModel::StopAnimation()
{
	for (int i = 0; i < vertexes.size(); i++) {
		vertexes[i] = vertexes[i];
	}
	currentAnimation = nullptr;	
}

AnimateModel::Animation * AnimateModel::getAnimation(std::string aname)
{
	for (auto a : animations) {
		if (a->GetName() == aname) {
			return a;
		}
	}
	return nullptr;
}

size_t AnimateModel::GetVertexesNum()
{
	return vertexes.size();
}

size_t AnimateModel::GetVertexSize()
{
	return sizeof(SmaVertex);
}

SmaVertex* AnimateModel::GetVertexes()
{
	return &vertexes[0];
}

