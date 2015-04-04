/*
	Project			 : Wolf Engine (http://WolfStudio.co). Copyright(c) Pooya Eimandar (http://PooyaEimandar.com) . All rights reserved.
	Source			 : https://github.com/PooyaEimandar/WolfEngine - Please direct any bug to hello@WolfStudio.co or tweet @PooyaEimandar on twitter
	Name			 : W_Game.h
	Description		 : The main class of game
	Comment          :
*/

#pragma once

#include "W_GraphicsDeviceManager.h"
#include <W_GameTime.h>

#ifdef __MAYA
#include "GraphicsResource/Textures/RenderTarget2D.h"
#endif // MAYA

namespace Wolf
{
	namespace Framework
	{
		//Provides basic graphics device initialization, load game assets, game logic and rendering code.
		class W_Game : public Wolf::Graphics::W_GraphicsDeviceManager
		{
		public:
			API W_Game();
			API virtual ~W_Game();

			//This will run the main loop cycle of the game 
			API bool Run(std::map<int, std::vector<W_WindowInfo>> pOutputWindowsInfo);
			API ULONG Release() override;

#pragma region Setters

			void SetAppName(std::string pAppName)							{ this->appName = pAppName; }

#pragma endregion

		protected:

			/*
				Allows the game to perform any initialization and it needs to before starting to run.
				Note that Log file will be created inside this function
			*/
			API virtual void Initialize(std::map<int, std::vector<W_WindowInfo>> pOutputWindowsInfo);

			//Load game assets
			API	virtual void Load();
			//Update the game logic
			API virtual void Update(System::W_GameTime pGameTime);
			//Draw the components of the game
			API virtual void Render(System::W_GameTime pGameTime);

		private:
			std::string											appName;
			Wolf::System::W_GameTime							gameTime;

#ifdef __MAYA
			std::unique_ptr<Wolf::Graphics::RenderTarget2D>		backBufferRenderTarget;
#endif // MAYA
		};
	}
}
