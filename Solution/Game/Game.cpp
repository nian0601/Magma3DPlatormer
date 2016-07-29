#include "stdafx.h"

#include <Rendering/AssetContainer.h>
#include <Rendering/Camera.h>
#include <Rendering/DeferredRenderer.h>
#include <Engine.h>
#include "Game.h"
#include <MathHelper.h>
#include <PostMaster.h>
#include <RenderMessage.h>
#include <Rendering/RendererProxy.h>
#include <XMLReader.h>

#include <RenderComponent.h>
#include <TranslationComponent.h>
#include <InputComponent.h>

#include <Entity/Include/TypeID.h>

#include <Entity/Include/ComponentFilter.h>
#include <RenderProcessor.h>
#include <InputProcessor.h>

Game::Game()
{
}


Game::~Game()
{
	PostMaster::GetInstance()->Destroy();
}

void Game::Init(Magma::Engine& aEngine)
{
	myCamera = &aEngine.GetCamera();
	myCamera->Move({ 0.f, 0.f, -30.f });

	myRendererProxy = &aEngine.GetRendererProxy();

	LoadLevel(aEngine.GetAssetContainer());

	myWorld.AddProcessor<RenderProcessor>();
	myWorld.AddProcessor<InputProcessor>();
}

bool Game::Update(float aDelta)
{
	UpdateCamera(aDelta);

	CU::InputWrapper* input = CU::InputWrapper::GetInstance();

	if (input->KeyIsPressed(DIK_ESCAPE))
	{
		return false;
	}

	myWorld.Update(aDelta);

	return true;
}

void Game::OnResize(float aWidth, float aHeight)
{
	//throw std::logic_error("The method or operation is not implemented.");
}

void Game::ReceiveMessage(const RenderMessage& aMessage)
{
	myRendererProxy->RenderModel(aMessage.myModelID, aMessage.myEffectID, aMessage.myOrientation, aMessage.myScale);
}

void Game::UpdateCamera(float aDelta)
{
	myCamera->Update();

	float speed = 10.f * aDelta;
	CU::InputWrapper* input = CU::InputWrapper::GetInstance();
	CU::Vector3<float> dir;

	if (input->KeyIsPressed(DIK_NUMPAD8))
	{
		dir.z = 1.f;
	}
	else if (input->KeyIsPressed(DIK_NUMPAD2))
	{
		dir.z = -1.f;
	}

	if (input->KeyIsPressed(DIK_NUMPAD4))
	{
		dir.x = -1.f;
	}
	else if (input->KeyIsPressed(DIK_NUMPAD6))
	{
		dir.x = 1.f;
	}

	if (input->KeyIsPressed(DIK_NUMPAD7))
	{
		dir.y = 1.f;
	}
	else if (input->KeyIsPressed(DIK_NUMPAD9))
	{
		dir.y = -1.f;
	}

	CU::Normalize(dir);
	myCamera->Move(dir * speed);

	if (input->KeyIsPressed(DIK_UP))
	{
		myCamera->Rotate(CU::Matrix44<float>::CreateRotateAroundX(-PI * 0.5f * aDelta));
	}
	else if (input->KeyIsPressed(DIK_DOWN))
	{
		myCamera->Rotate(CU::Matrix44<float>::CreateRotateAroundX(PI * 0.5f * aDelta));
	}
	else if (input->KeyIsPressed(DIK_LEFT))
	{
		myCamera->Rotate(CU::Matrix44<float>::CreateRotateAroundY(-PI * 0.5f * aDelta));
	}
	else if (input->KeyIsPressed(DIK_RIGHT))
	{
		myCamera->Rotate(CU::Matrix44<float>::CreateRotateAroundY(PI * 0.5f * aDelta));
	}
}

void Game::LoadLevel(Magma::AssetContainer& aAssetContainer)
{
	Magma::Entity lastEntity = 0;
	XMLReader reader;
	reader.OpenDocument("Data/Resource/Level/Level_01.xml");
	for (tinyxml2::XMLElement* cube = reader.ForceFindFirstChild("Cube"); cube != nullptr; cube = reader.FindNextElement(cube, "Cube"))
	{
		CU::Vector3<float> position;
		CU::Vector3<float> rotation;
		CU::Vector3<float> scale;

		reader.ForceReadAttribute(reader.ForceFindFirstChild(cube, "Position"), "x", "y", "z", position);
		reader.ForceReadAttribute(reader.ForceFindFirstChild(cube, "Scale"), "x", "y", "z", scale);
		reader.ForceReadAttribute(reader.ForceFindFirstChild(cube, "Rotation"), "x", "y", "z", rotation);

		rotation.x = CU::Math::DegreeToRad(rotation.x);
		rotation.y = CU::Math::DegreeToRad(rotation.y);
		rotation.z = CU::Math::DegreeToRad(rotation.z);


		Magma::Entity entity = myWorld.CreateEntity();
		myWorld.AddComponent<TranslationComponent>(entity);
		myWorld.AddComponent<RenderComponent>(entity);
		TranslationComponent& comp = myWorld.GetComponent<TranslationComponent>(entity);
		comp.myScale = scale;
		
		comp.myOrientation = comp.myOrientation * CU::Matrix44f::CreateRotateAroundZ(rotation.z);
		comp.myOrientation = comp.myOrientation * CU::Matrix44f::CreateRotateAroundX(rotation.x);
		comp.myOrientation = comp.myOrientation * CU::Matrix44f::CreateRotateAroundY(rotation.y);

		comp.myOrientation.SetPos(position);

		RenderComponent& renderComp = myWorld.GetComponent<RenderComponent>(entity);
		renderComp.myModelID = aAssetContainer.LoadModel("Data/Resource/Model/Sphere/Sphere.fbx", "Data/Resource/Shader/S_effect_cube.fx");
		renderComp.myEffectID = aAssetContainer.LoadEffect("Data/Resource/Shader/S_effect_cube.fx");

		lastEntity = entity;
	}

	myWorld.AddComponent<InputComponent>(lastEntity);

	reader.CloseDocument();
}
