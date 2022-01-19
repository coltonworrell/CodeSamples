/*HEADER_GOES_HERE*/
#include "../../Headers/Components/Editor.h"
#include "../../Headers/Containers/Object.h"
#include "../../Headers/Containers/Space.h"
#include "../../Headers/Factory.h"
#include "imgui/imgui.h"
#include <sstream>

#include "../../Headers/Events/UpdateEvent.h"
#include "../../Headers/Events/DrawEvent.h"
#include "../../Headers/Events/TraceEvent.h"

std::string Editor::GetName() { return "Editor"; }

std::map<std::string, std::array<bool, COMPONENT_COUNT + 1>> dropDowns;

std::unique_ptr<Editor> Editor::RegisterEditor() 
{
  auto component = std::unique_ptr<Editor>(new Editor(
    //5999                                            /***Only needed if you don't provide a default constructor***/
  ));

  return std::move(component);
}

Editor::Editor(
  //Int componentData_
) : Component(componentType) //, componentData(componentData_)
{
  static bool registered = false;
  if (!registered)
  {
    Factory::ComponentPropertyMap["Editor"] = std::vector<PropertyID>({
      //PROPERTY_ID(Int, componetData)
      });
    registered = true;
  }
}

Editor::~Editor()
{
}

void Editor::Init()
{
  //GetSpace()->RegisterListener(this, &Editor::UpdateEventsListen);
  GetSpace()->RegisterListener(this, &Editor::DrawEventsListen);
}

void Editor::End()
{
  if (parent != nullptr)
  {
    //GetSpace()->UnregisterListener(this, &Editor::UpdateEventsListen);
    GetSpace()->UnregisterListener(this, &Editor::DrawEventsListen);
  }
}

std::unique_ptr<Component> Editor::Clone() const
{
  auto result = new Editor();
    //copy over values here
  //result->componetData = componetData;

  return std::unique_ptr<Component>(result);
}

void Editor::UpdateEventsListen(UpdateEvent* updateEvent) 
{
}

void Editor::DrawEventsListen(DrawEvent* drawEvent) 
{
  ImGui::SetNextWindowPos({ 900, 20 });
  ImGui::SetNextWindowSize({ 300, 800 });
  ImGui::Begin("Editor"
    , NULL
    , 0
  );

  for (auto& obj : GetSpace()->GetObjects())
  {
    if (obj->isTemp)
      continue;
    if (dropDowns.find(obj->GetName()) == dropDowns.end())
    {
      dropDowns[obj->GetName()] = std::array<bool, COMPONENT_COUNT + 1>();
    }
    ImGui::Checkbox((std::stringstream() << obj->GetName() + "##" << &*obj).str().c_str(), &dropDowns[obj->GetName()][COMPONENT_COUNT]);
    if (dropDowns[obj->GetName()][COMPONENT_COUNT])
    {
      for (ID i = 0; i < COMPONENT_COUNT; ++i)
      {
        Component* comp = obj->FindComponent(i);
        if (comp != nullptr)
        {
          ImGui::Checkbox((std::stringstream() << comp->GetName() + "##" << &*comp).str().c_str(), &dropDowns[obj->GetName()][i]);
          if (dropDowns[obj->GetName()][i])
          {
            auto& vectorOfPropertyIDs = Factory::ComponentPropertyMap[comp->GetName()];
            for (auto& propertyID : vectorOfPropertyIDs)
            {
              Factory::GuiIfy(comp, propertyID);
            }
            comp->EditorVerify();
          }
        }
      }
    }
  }

  ImGui::End();
}
