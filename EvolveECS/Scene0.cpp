#include <glew.h>
#include <conio.h>
#include <iostream>
#include <SDL.h>
#include "Debug.h"
#include "Scene0.h"
#include <MMath.h>
#include "Debug.h"
#include "TransformComponent.h"
#include "MaterialComponent.h"
#include <QMath.h>
#include "XMLAssetManager.h"
#include "ShaderComponent.h"
#include "MeshComponent.h"
#include "ShapeComponent.h"
#include "QuadraticSolver.h"
#include "Physics.h"


bool Scene0::OnCreate()
{
	XMLAssetManager assetManager;
	// Make sure these names match the stuff in your xml file:
	std::vector<std::string> names{ 
		"ActorGameBoard", /*"ActorChecker1",*/ "ActorChecker2",
		"ActorDebug",
		"ActorSkull", "ActorCube", "ActorMario"
	};
	for (const auto& name : names) {
		auto asset = assetManager.xmlAssets.find(name);
		actors[name] = asset->second;
	}
	camera = std::dynamic_pointer_cast<CameraActor>(assetManager.xmlAssets.find("Camera1")->second);
	light = std::dynamic_pointer_cast<LightActor>(assetManager.xmlAssets.find("Light1")->second);

	for (auto it = actors.begin(); it != actors.end(); ++it) {
		Ref<Actor> actor = std::dynamic_pointer_cast<Actor>(it->second);
		Ref<TransformComponent> transformComponent = actor->GetComponent <TransformComponent>();
		Ref<PhysicsComponent> physicsComponent = actor->GetComponent <PhysicsComponent>();

		// Fixes orientation
		Matrix3 inverseRotationalMatrix_ = MMath::inverse(MMath::toMatrix3(transformComponent->orientation));
		physicsComponent->inverseRotationMatrix = inverseRotationalMatrix_;
	}

	return true;
}

void Scene0::OnDestroy()
{
	actors.clear();
}


void Scene0::HandleEvents(const SDL_Event& sdlEvent)
{
	Ref<TransformComponent> cameraTransform = camera->GetComponent <TransformComponent>();
	switch (sdlEvent.type) {
	case SDL_KEYDOWN:
		if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_LEFT) {

		}
		else if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_RIGHT) {

		}
		else if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_UP) {

		}
		else if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_DOWN) {

		}
		else if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_D) {
			cameraTransform->SetTransform(cameraTransform->pos, cameraTransform->GetOrientation() * QMath::angleAxisRotation(-2.0f, Vec3(0.0f, 1.0f, 0.0f)));
			camera->UpdateViewMatrix();
		}
		else if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_A) {
			cameraTransform->SetTransform(cameraTransform->pos, cameraTransform->GetOrientation() * QMath::angleAxisRotation(2.0f, Vec3(0.0f, 1.0f, 0.0f)));
			camera->UpdateViewMatrix();

		}
	
		else if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_W) {
			cameraTransform->SetTransform(cameraTransform->pos, cameraTransform->GetOrientation() * QMath::angleAxisRotation(2.0f, Vec3(1.0f, 0.0f, 0.0f)));
			camera->UpdateViewMatrix();

		}
		else if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_S) {
			cameraTransform->SetTransform(cameraTransform->pos, cameraTransform->GetOrientation() * QMath::angleAxisRotation(-2.0f, Vec3(1.0f, 0.0f, 0.0f)));
			camera->UpdateViewMatrix();

		}
		else if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_M) {
			renderMeshes = !renderMeshes;
		}
		else if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_C) {
			renderCollisionShapes = !renderCollisionShapes;
		}

		break;
		
	case SDL_MOUSEBUTTONDOWN:
		if (sdlEvent.button.button == SDL_BUTTON_LEFT) {

			std::cout << std::string(100, '\n');

			Vec4 mouseCoords(static_cast<float>(sdlEvent.button.x), static_cast<float>(sdlEvent.button.y), 0.0f, 1.0f);

			Matrix4 NDCToPixelSpace = MMath::viewportNDC(1280, 720);
			Vec4 mouseNDCCoords = MMath::inverse(NDCToPixelSpace) * mouseCoords;
			mouseNDCCoords.z = -1.0f;

			Matrix4 perspectiveToNdc = camera->GetProjectionMatrix();
			Vec4 mouseperspectiveCoords = MMath::inverse(perspectiveToNdc) * mouseNDCCoords;
			mouseperspectiveCoords /= mouseperspectiveCoords.w;
			
			Matrix4 worldToSpace = camera->GetViewMatrix();
			Vec4 mouseWorldCoords = MMath::inverse(worldToSpace) * mouseperspectiveCoords;

			/// Ray starts at the camera world position
			Vec3 rayStart = camera->GetComponent<TransformComponent>()->pos;
			///Ray direction is from the camera position to the front clipping plane
			Vec3 rayDir = mouseWorldCoords - rayStart;


			// Loop through all the actors and check if the ray has collided with them
			// Iterate over each actor to detect ray collisions, selecting the closest one
			for (auto& actorPair : actors) {
				auto actor = std::dynamic_pointer_cast<Actor>(actorPair.second);
				auto transformComponent = actor->GetComponent<TransformComponent>();
				auto shapeComponent = actor->GetComponent<ShapeComponent>();

				// Transform the ray to the object's local space and check for a collision
				auto worldToLocalMatrix = MMath::inverse(actor->GetModelMatrix());
				auto localRayStart = worldToLocalMatrix * Vec4(rayStart, 1.0f);
				auto localRayDir = worldToLocalMatrix * Vec4(rayDir, 0.0f);
				GEOMETRY::Ray localRay(localRayStart, localRayDir);

				auto rayInfo = shapeComponent->shape->rayIntersectionInfo(localRay);
				if (rayInfo.isIntersected) {
					if (actorPair.first != "ActorDebug") {
						std::cout << "You picked: " << actorPair.first << '\n';
						pickedActor = actor;
						haveClickedOnSomething = true;
						intersectionPoint = transformComponent->GetTransformMatrix() * rayInfo.intersectoinPoint;

						auto physicsComponent = actor->GetComponent<PhysicsComponent>();
						physicsComponent->inverseRotationMatrix = MMath::inverse(MMath::toMatrix3(transformComponent->orientation));

						// Update the debug actor's position and scale
						auto debugTransform = std::dynamic_pointer_cast<Actor>(actors["ActorDebug"])->GetComponent<TransformComponent>();
						debugTransform->pos = intersectionPoint;
						debugTransform->scale = Vec3(0.5, 0.5f, 0.5f);
					}
				}
			}

		}
		break;
	default:
		break;
	}

}

void Scene0::Update(const float deltaTime)
{	
	if (haveClickedOnSomething) {
		Ref<PhysicsComponent> body = pickedActor->GetComponent<PhysicsComponent>();
		ShapeType shape = pickedActor->GetComponent<ShapeComponent>()->shapeType;
		// Set up gravity and drag forces
		Vec3 gravityForce(0.0f, -9.8f * body->mass, 0.0f);
		float dragCoeff = 0.25f;
		Vec3 dragForce = body->vel * (-dragCoeff);
		Vec3 netForce = gravityForce + dragForce;
		Physics::ApplyForce(body, netForce);

		/*
		* update the position and velocity in separate steps
		*/
		Physics::UpdateVel(body, deltaTime);

		if (shape == ShapeType::sphere) {
			// The plane is defined by a normal and a distance from the origin
			Ref<Actor> gameBoard = std::dynamic_pointer_cast<Actor>(actors.find("ActorGameBoard")->second);
			Ref<ShapeComponent> shapeComponent = pickedActor->GetComponent<ShapeComponent>();
			Vec3 planeNormal = VMath::normalize(Vec3(0.0f, 1.0f, 1.0f));
			Vec3 pointOnPlane = gameBoard->GetComponent<TransformComponent>()->pos;

			float planeDistance = VMath::dot(planeNormal, pointOnPlane);
			float radius = std::dynamic_pointer_cast<GEOMETRY::Sphere>(shapeComponent->shape)->r;
			planeDistance += radius; // Adjust for sphere radius
			// Calculate the angular velocity using the velocity and the radius of skull
			float angularVelMagnitude = VMath::mag(body->vel) / radius;
			/**
			* the axis of rotation is the cross product of the plane normal and the velocity
			* e.g. if the plane normal is (0, 1, 0) and the velocity is (1, 0, 0)
			* the axis of rotation is (0, 0, -1)
			* can test it by using the right hand rule
			*/
			Vec3 axisOfRotation = VMath::normalize(VMath::cross(planeNormal, body->vel));
			body->angularVel = angularVelMagnitude * axisOfRotation;

			Physics::PlaneConstraint(body, deltaTime, planeNormal, planeDistance);
			}
			else if (shape == ShapeType::box) {
				Physics::MouseConstraint(body, deltaTime, intersectionPoint);
			}
			else if (shape == ShapeType::capsule) {
				Physics::MouseConstraint(body, deltaTime, intersectionPoint);
			}
			else if (shape == ShapeType::cylinder) {
				float slope = -1.0f;
				float yIntercept = 20.0f;
				Physics::StraightLineConstraint(body, deltaTime, slope, yIntercept);
			}
			Physics::UpdatePos(body, deltaTime);
			Physics::UpdateOrientation(body, deltaTime);
			// Ensure the actor’s transform component matches the physics component
			Physics::UpdateTransform(pickedActor);
	}
}


void Scene0::Render() const
{
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBindBuffer(GL_UNIFORM_BUFFER, camera->GetMatricesID());
	glBindBuffer(GL_UNIFORM_BUFFER, light->GetLightID());
	// Let it go
	glBindTexture(GL_TEXTURE_2D, 0);

	for (auto it = actors.begin(); it != actors.end(); ++it) {
		Ref<Actor> actor = std::dynamic_pointer_cast<Actor>(it->second);
		if (camera->isInBoxView(actor->GetComponent<TransformComponent>()->pos)) {
			// Draw mesh
			glUseProgram(actor->GetComponent<ShaderComponent>()->GetProgram());
			glUniformMatrix4fv(actor->GetComponent<ShaderComponent>()->GetUniformID("modelMatrix"), 1, GL_FALSE, actor->GetModelMatrix());
			glBindTexture(GL_TEXTURE_2D, actor->GetComponent<MaterialComponent>()->getTextureID());
			if (renderMeshes) {
				actor->GetComponent<MeshComponent>()->Render(GL_TRIANGLES);
			}
			if (renderCollisionShapes) {
				// Drawing the primitive geometry associated with the mesh to help debug ray intersects, culling, and collision detection
				actor->GetComponent<ShapeComponent>()->Render();
			}
		}
	}
}

