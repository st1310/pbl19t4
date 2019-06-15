#pragma once

#include "DrawableGameComponent.h"
#include "KeyboardComponent.h"
#include "SerializableGameObject.h"
#include "RenderStateHelper.h"
#include "SpriteBatch.h"
#include "SpriteFont.h"
#include "NodeList.h"
#include "Game.h"
#include <iomanip> 
#include <fstream>
#include <sstream>
#include "SpotLight.h"

namespace Library
{
	class DirectionalLight;
	class PointLight;
	class SpotLight;
	class ProxyModel;

	class Scene : public DrawableGameComponent
{
		RTTI_DECLARATIONS(Scene, DrawableGameComponent)

	public:
		Scene(Game& game, Camera& camera, int sceneId, std::string filePath);
		~Scene();		

		virtual void Start(Game& game, Camera& camera);
		virtual void Update(const GameTime& gameTime);
		void Initialize();
		void Clear();

		int SceneId;
		
		std::vector<GameComponent*> GameObjects;
		void SaveToFile();
		std::vector<SerializableGameObject> LoadFromFile();

		void rewriteListOfNodes(std::vector<CollisionNode*>& list);
		std::vector<CollisionNode*> getListOfNode();
		std::vector<DrawableGameComponent*> GetPolicemans();
		 
		void ClearUnitList();
		void AddUnitToList(DrawableGameComponent* unit);
		void RewriteUnitList(std::vector<DrawableGameComponent*> newListOfUnits);
		void RemoveUnitFromList(DrawableGameComponent* unit);
		std::vector<DrawableGameComponent*> GetUnitList();
		
		void AddTriggerableObjectToList(DrawableGameComponent* object);
		void RemoveTriggerableObjectFromList(DrawableGameComponent* object);
		std::vector<DrawableGameComponent*> GetTriggerableObjects();

		Colliders* GetGroundCollider();
		void SetGroudndCollider(Colliders* collider);
		
		void BuildNodesStart(XMFLOAT3 posA, XMFLOAT3 posC);

		std::vector<DirectionalLight*>& GetDirectionalLights();
		std::vector<PointLight*>& GetPointLights();
		std::vector<SpotLight*>& GetSpotLights();
		void ResetLights();
	protected:
		KeyboardComponent* mKeyboard;
		RenderStateHelper mRenderStateHelper;
		SpriteBatch* mSpriteBatch;
		SpriteFont* mSpriteFont;
		XMFLOAT2 mTextPosition;

		std::vector<DrawableGameComponent*> listOfUnits;
		std::vector<DrawableGameComponent*> mPolicemans;
		std::vector<DrawableGameComponent*> trigerrableObjects;

		std::vector<DirectionalLight*> mDirectLights;
		std::vector<PointLight*> mPointLights;
		std::vector<SpotLight*> mSpotLights;

		Colliders* groundCollider;
		void InitializeLights();
	private:
		void BuildNodesRec(CollisionNode* parNode);

		std::string mFileName;
		std::vector<CollisionNode*> listOfNodes;
	};
}

