#include "XMLAssetManager.h"
#include <iostream>
#include <fstream>
#include <string>
#include "ShaderComponent.h"
#include "MeshComponent.h"
#include "ShapeComponent.h"
#include "MaterialComponent.h"
#include "CameraActor.h"
#include "LightActor.h"
#include "Sphere.h"
#include <QMath.h>
#include "Cylinder.h"
#include "Capsule.h"

XMLAssetManager::XMLAssetManager()
{
	tinyxml2::XMLDocument doc;
	doc.LoadFile("Scene0.xml");
	bool status = doc.Error();
	if (status) {
		std::cout << doc.ErrorIDToName(doc.ErrorID()) << std::endl;
		return;
	}
	// These are referred to as nodes. Get the top of the system
	tinyxml2::XMLElement* rootData = doc.RootElement();
	tinyxml2::XMLElement* sceneData = rootData->FirstChildElement("Scene0");

	for (const tinyxml2::XMLElement* child = rootData->FirstChildElement("Scene0")->FirstChildElement();
		child;
		child = child->NextSiblingElement())
	{
		AddMaterial(child);
		AddShader(child);
		AddCamera(child);
		AddLight(child);
		
		if (std::string(child->Name()) == "Mesh") {
			AddComponent<MeshComponent>(child->Attribute("name"), nullptr, child->Attribute("filename"));
			
			AddSphereShape(child);
			AddCylinderShape(child);
			AddCapsuleShape(child);
			AddBoxShape(child);
		}
	}

	// Now we have all the shared assets ready, time to build the actors
	for (const tinyxml2::XMLElement* child = rootData->FirstChildElement("Scene0")->FirstChildElement("Actor");
		child;
		child = child->NextSiblingElement())
	{
		if (std::string(child->Name()) == "Actor") {
			Component* parent = nullptr;
			if (std::string(child->Attribute("parent")) == "none") {
				parent = nullptr;
			}
			if (std::string(child->Attribute("parent")) == "ActorGameBoard") {
				parent = GetComponent<Actor>("ActorGameBoard").get();
			}

			Ref<Actor> actor = std::make_shared<Actor>(parent);
			// Add shared assets to the actor
			AddMeshToActor(child, actor);
			AddShaderToActor(child, actor);
			AddMaterialToActor(child, actor);
			AddShapeToActor(child, actor);
			// The transform is unique for the actor. Needs the parent too
			AddTransformToActor(child, actor, parent);
			// Add physics to the actor AFTER the transform. We need them to match in position and orientation
			AddPhysicsToActor(child, actor, parent);

			actor->OnCreate();
			AddComponent(child->Attribute("actorname"), actor);
		}
	}
}
XMLAssetManager::~XMLAssetManager()
{
	xmlAssets.clear();
}

void XMLAssetManager::AddSphereShape(const tinyxml2::XMLElement* child)
{
	if (std::string(child->FirstChildElement("Shape")->FirstChildElement()->Name()) == "Sphere") {
		float sphereRadius = child->FirstChildElement("Shape")->FirstChildElement("Sphere")->FloatAttribute("radius");
		
		Vec3 sphereCentre;
		sphereCentre.x = child->FirstChildElement("Shape")->FirstChildElement("Sphere")->FloatAttribute("centreX");
		sphereCentre.y = child->FirstChildElement("Shape")->FirstChildElement("Sphere")->FloatAttribute("centreY");
		sphereCentre.z = child->FirstChildElement("Shape")->FirstChildElement("Sphere")->FloatAttribute("centreZ");

		GEOMETRY::Sphere sphere;
		sphere.set(sphereCentre.x, sphereCentre.y, sphereCentre.z, sphereRadius);

		AddComponent<ShapeComponent>(child->FirstChildElement("Shape")->Attribute("name"), nullptr, sphere);
	}
}

void XMLAssetManager::AddCylinderShape(const tinyxml2::XMLElement* child)
{
	if (std::string(child->FirstChildElement("Shape")->FirstChildElement()->Name()) == "Cylinder") {
		float cylinderRadius = child->FirstChildElement("Shape")->FirstChildElement("Cylinder")->FloatAttribute("radius");

		Vec3 capCentrePosA;
		capCentrePosA.x = child->FirstChildElement("Shape")->FirstChildElement("Cylinder")->FloatAttribute("capCentrePosAx");
		capCentrePosA.y = child->FirstChildElement("Shape")->FirstChildElement("Cylinder")->FloatAttribute("capCentrePosAy");
		capCentrePosA.z = child->FirstChildElement("Shape")->FirstChildElement("Cylinder")->FloatAttribute("capCentrePosAz");

		Vec3 capCentrePosB;
		capCentrePosB.x = child->FirstChildElement("Shape")->FirstChildElement("Cylinder")->FloatAttribute("capCentrePosBx");
		capCentrePosB.y = child->FirstChildElement("Shape")->FirstChildElement("Cylinder")->FloatAttribute("capCentrePosBy");
		capCentrePosB.z = child->FirstChildElement("Shape")->FirstChildElement("Cylinder")->FloatAttribute("capCentrePosBz");

		GEOMETRY::Cylinder cylinder;
		cylinder.set(cylinderRadius, capCentrePosA, capCentrePosB);

		AddComponent<ShapeComponent>(child->FirstChildElement("Shape")->Attribute("name"), nullptr, cylinder);
	}
}

void XMLAssetManager::AddCapsuleShape(const tinyxml2::XMLElement* child)
{
	if (std::string(child->FirstChildElement("Shape")->FirstChildElement()->Name()) == "Capsule") {
		float capsuleRadius = child->FirstChildElement("Shape")->FirstChildElement("Capsule")->FloatAttribute("radius");

		Vec3 sphereCentrePosA;
		sphereCentrePosA.x = child->FirstChildElement("Shape")->FirstChildElement("Capsule")->FloatAttribute("sphereCentrePosAx");
		sphereCentrePosA.y = child->FirstChildElement("Shape")->FirstChildElement("Capsule")->FloatAttribute("sphereCentrePosAy");
		sphereCentrePosA.z = child->FirstChildElement("Shape")->FirstChildElement("Capsule")->FloatAttribute("sphereCentrePosAz");

		Vec3 sphereCentrePosB;
		sphereCentrePosB.x = child->FirstChildElement("Shape")->FirstChildElement("Capsule")->FloatAttribute("sphereCentrePosBx");
		sphereCentrePosB.y = child->FirstChildElement("Shape")->FirstChildElement("Capsule")->FloatAttribute("sphereCentrePosBy");
		sphereCentrePosB.z = child->FirstChildElement("Shape")->FirstChildElement("Capsule")->FloatAttribute("sphereCentrePosBz");

		GEOMETRY::Capsule capsule;
		capsule.set(capsuleRadius, sphereCentrePosA, sphereCentrePosB);

		AddComponent<ShapeComponent>(child->FirstChildElement("Shape")->Attribute("name"), nullptr, capsule);
	}
}

void XMLAssetManager::AddBoxShape(const tinyxml2::XMLElement* child)
{
	// TODO for assignment 1
}

void XMLAssetManager::AddMaterial(const tinyxml2::XMLElement* child)
{
	if (std::string(child->Name()) == "Material") {
		AddComponent<MaterialComponent>(child->Attribute("name"), nullptr, child->Attribute("filename"));
	}
}

void XMLAssetManager::AddShader(const tinyxml2::XMLElement* child)
{
	if (std::string(child->Name()) == "Shader") {
		AddComponent<ShaderComponent>(child->Attribute("name"), nullptr, child->Attribute("vertFilename"), child->Attribute("fragFilename"));
	}
}

void XMLAssetManager::AddCamera(const tinyxml2::XMLElement* child)
{
	// Build camera
	Vec3 cameraPos;
	Vec3 axis;
	Vec3 scale;
	float angleDeg = 0.0f;
	Component* cameraParent = nullptr;

	if (child->Attribute("cameraname")) {
		if (child->FirstChildElement("Transform")) {
			cameraPos.x = child->FirstChildElement("Transform")->FloatAttribute("posx");
			cameraPos.y = child->FirstChildElement("Transform")->FloatAttribute("posy");
			cameraPos.z = child->FirstChildElement("Transform")->FloatAttribute("posz");
			angleDeg = child->FirstChildElement("Transform")->FloatAttribute("angleDeg");;
			axis.x = child->FirstChildElement("Transform")->FloatAttribute("axisx");
			axis.y = child->FirstChildElement("Transform")->FloatAttribute("axisy");
			axis.z = child->FirstChildElement("Transform")->FloatAttribute("axisz");
			scale.x = child->FirstChildElement("Transform")->FloatAttribute("scalex");
			scale.y = child->FirstChildElement("Transform")->FloatAttribute("scaley");
			scale.z = child->FirstChildElement("Transform")->FloatAttribute("scalez");

			if (std::string(child->Attribute("parent")) == "none") {
				cameraParent = nullptr;
			}
		}
	}

	if (std::string(child->Name()) == "Camera") {
		Ref<CameraActor> camera = std::make_shared<CameraActor>(cameraParent);
		camera->AddComponent<TransformComponent>(nullptr, cameraPos, QMath::angleAxisRotation(angleDeg, axis));
		camera->OnCreate();
		AddComponent(child->Attribute("cameraname"), camera);
	}
}

void XMLAssetManager::AddLight(const tinyxml2::XMLElement* child)
{
	// Build Light
	Vec3 lightPos;
	Vec4 colour;
	Vec3 falloff;
	Component* lightParent = nullptr;
	LightStyle lightstyle;
	float intensity = 1.0f;
	if (child->Attribute("lightname")) {
		lightPos.x = child->FloatAttribute("posx");
		lightPos.y = child->FloatAttribute("posy");
		lightPos.z = child->FloatAttribute("posz");

		colour.x = child->FloatAttribute("red");
		colour.y = child->FloatAttribute("green");
		colour.z = child->FloatAttribute("blue");
		colour.w = child->FloatAttribute("alpha");

		intensity = child->FloatAttribute("intensity");;

		falloff.x = child->FloatAttribute("falloffx");
		falloff.y = child->FloatAttribute("falloffy");
		falloff.z = child->FloatAttribute("falloffz");

		if (std::string(child->Attribute("parent")) == "none") {
			lightParent = nullptr;
		}

		if (std::string(child->Attribute("lightstyle")) == "DirectionLight") {
			lightstyle = LightStyle::DirectionLight;
		}

	}

	if (std::string(child->Name()) == "Light") {
		Ref<LightActor> light = std::make_shared<LightActor>(lightParent, lightstyle, lightPos, colour, intensity, falloff);
		light->OnCreate();
		AddComponent(child->Attribute("lightname"), light);
	}
}

void XMLAssetManager::AddMeshToActor(const tinyxml2::XMLElement* child, Ref<Actor> actor)
{
	if (child->FirstChildElement("Mesh")) {
		const char* name = child->FirstChildElement("Mesh")->Attribute("name");
		Ref<MeshComponent> mesh = GetComponent<MeshComponent>(name);
		actor->AddComponent<MeshComponent>(mesh);
	}

}

void XMLAssetManager::AddShaderToActor(const tinyxml2::XMLElement* child, Ref<Actor> actor)
{
	if (child->FirstChildElement("Shader")) {
		const char* name = child->FirstChildElement("Shader")->Attribute("name");
		Ref<ShaderComponent> shader = GetComponent<ShaderComponent>(name);
		actor->AddComponent<ShaderComponent>(shader);
	}
}

void XMLAssetManager::AddMaterialToActor(const tinyxml2::XMLElement* child, Ref<Actor> actor)
{
	if (child->FirstChildElement("Material")) {
		const char* name = child->FirstChildElement("Material")->Attribute("name");
		Ref<MaterialComponent> material = GetComponent<MaterialComponent>(name);
		actor->AddComponent<MaterialComponent>(material);
	}
}

void XMLAssetManager::AddShapeToActor(const tinyxml2::XMLElement* child, Ref<Actor> actor)
{
	if (std::string(child->FirstChildElement("Shape")->Name()) == "Shape") {
		const char* name = child->FirstChildElement("Shape")->Attribute("name");
		Ref<ShapeComponent> shape = GetComponent<ShapeComponent>(name);
		actor->AddComponent<ShapeComponent>(shape);
	}
}

void XMLAssetManager::AddTransformToActor(const tinyxml2::XMLElement* child, Ref<Actor> actor, Component* parent)
{
	if (child->FirstChildElement("Transform")) {
		Vec3 pos;
		Vec3 axis;
		Vec3 scale;
		pos.x = child->FirstChildElement("Transform")->FloatAttribute("posx");
		pos.y = child->FirstChildElement("Transform")->FloatAttribute("posy");
		pos.z = child->FirstChildElement("Transform")->FloatAttribute("posz");
		float angleDeg = child->FirstChildElement("Transform")->FloatAttribute("angleDeg");;
		axis.x = child->FirstChildElement("Transform")->FloatAttribute("axisx");
		axis.y = child->FirstChildElement("Transform")->FloatAttribute("axisy");
		axis.z = child->FirstChildElement("Transform")->FloatAttribute("axisz");
		scale.x = child->FirstChildElement("Transform")->FloatAttribute("scalex");
		scale.y = child->FirstChildElement("Transform")->FloatAttribute("scaley");
		scale.z = child->FirstChildElement("Transform")->FloatAttribute("scalez");
		
		actor->AddComponent<TransformComponent>(parent, pos, QMath::angleAxisRotation(angleDeg, axis), scale);
	}
}

void XMLAssetManager::AddPhysicsToActor(const tinyxml2::XMLElement* child, Ref<Actor> actor, Component* parent)
{
	// TODO for assignment 3
	// Add a default physics component to the actor, but don't forget to match transform component's position & orientation
	// You're gonna have to assume the transform component has already been built (fingers crossed!)
}

