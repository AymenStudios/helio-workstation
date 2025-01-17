/*
    This file is part of Helio Workstation.

    Helio is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Helio is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Helio. If not, see <http://www.gnu.org/licenses/>.
*/

//[Headers]
#include "Common.h"
//[/Headers]

#include "OrchestraPitPage.h"

//[MiscUserDefs]
#include "PluginScanner.h"
#include "OrchestraPitNode.h"
#include "MainLayout.h"
#include "AudioCore.h"
#include "MenuItemComponent.h"
#include "Icons.h"
#include "ProgressTooltip.h"
#include "CommandIDs.h"
#include "Workspace.h"
#include "ComponentIDs.h"
//[/MiscUserDefs]

OrchestraPitPage::OrchestraPitPage(PluginScanner &pluginScanner, OrchestraPitNode &instrumentsRoot)
    : pluginScanner(pluginScanner),
      instrumentsRoot(instrumentsRoot)
{
    this->skew.reset(new SeparatorVerticalSkew());
    this->addAndMakeVisible(skew.get());
    this->backgroundA.reset(new PanelBackgroundA());
    this->addAndMakeVisible(backgroundA.get());
    this->backgroundB.reset(new PanelBackgroundB());
    this->addAndMakeVisible(backgroundB.get());
    this->pluginsList.reset(new AudioPluginsListComponent(pluginScanner, instrumentsRoot));
    this->addAndMakeVisible(pluginsList.get());
    this->instrumentsList.reset(new InstrumentsListComponent(pluginScanner, instrumentsRoot));
    this->addAndMakeVisible(instrumentsList.get());

    //[UserPreSize]
    this->setComponentID(ComponentIDs::orchestraPit);
    //[/UserPreSize]

    this->setSize(600, 400);

    //[Constructor]
    this->pluginScanner.addChangeListener(this);
    this->instrumentsRoot.addChangeListener(this);
    //[/Constructor]
}

OrchestraPitPage::~OrchestraPitPage()
{
    //[Destructor_pre]
    this->instrumentsRoot.removeChangeListener(this);
    this->pluginScanner.removeChangeListener(this);
    //[/Destructor_pre]

    skew = nullptr;
    backgroundA = nullptr;
    backgroundB = nullptr;
    pluginsList = nullptr;
    instrumentsList = nullptr;

    //[Destructor]
    //[/Destructor]
}

void OrchestraPitPage::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void OrchestraPitPage::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    skew->setBounds(0 + (getWidth() - 510), 0, 64, getHeight() - 0);
    backgroundA->setBounds(0, 0, getWidth() - 510, getHeight() - 0);
    backgroundB->setBounds(getWidth() - 446, 0, 446, getHeight() - 0);
    pluginsList->setBounds(14, 10, (getWidth() - 510) - 16, getHeight() - 20);
    instrumentsList->setBounds((getWidth() - 446) + 14, 10, 446 - 28, getHeight() - 20);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void OrchestraPitPage::handleCommandMessage (int commandId)
{
    //[UserCode_handleCommandMessage] -- Add your code here...
    if (commandId == CommandIDs::ScanAllPlugins)
    {
        auto tooltip = MakeUnique<ProgressTooltip>(true);
        tooltip->onCancel = [this]() {
            this->pluginScanner.cancelRunningScan();
        };

        App::showModalComponentUnowned(tooltip.release());

        this->pluginScanner.runInitialScan();
    }
    else if (commandId == CommandIDs::ScanPluginsFolder)
    {
#if HELIO_DESKTOP
        FileChooser fc(TRANS(I18n::Dialog::scanFolderCaption),
            File::getCurrentWorkingDirectory(), ("*.*"), true);

        if (fc.browseForDirectory())
        {
            auto tooltip = MakeUnique<ProgressTooltip>(true);
            tooltip->onCancel = [this]() {
                this->pluginScanner.cancelRunningScan();
            };

            App::showModalComponentUnowned(tooltip.release());

            this->pluginScanner.scanFolderAndAddResults(fc.getResult());
        }
#endif
    }
    //[/UserCode_handleCommandMessage]
}

//===----------------------------------------------------------------------===//
// ChangeListener
//===----------------------------------------------------------------------===//

void OrchestraPitPage::changeListenerCallback(ChangeBroadcaster *source)
{
    this->pluginsList->showScanButtonIf(this->pluginScanner.getNumPlugins() == 0);
    this->pluginsList->updateListContent();
    this->instrumentsList->updateListContent();

    App::Layout().hideSelectionMenu();

    if (dynamic_cast<PluginScanner *>(source) &&
        !this->pluginScanner.isWorking())
    {
        // hides the spinner, if any
        App::dismissAllModalComponents();
    }
}

void OrchestraPitPage::onStageSelectionChanged()
{
    this->pluginsList->clearSelection();
}

void OrchestraPitPage::onPluginsSelectionChanged()
{
    this->instrumentsList->clearSelection();
}

//[/MiscUserCode]

#if 0
/*
BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="OrchestraPitPage" template="../../../Template"
                 componentName="" parentClasses="public Component, public ChangeListener"
                 constructorParams="PluginScanner &amp;pluginScanner, OrchestraPitNode &amp;instrumentsRoot"
                 variableInitialisers="pluginScanner(pluginScanner),&#10;instrumentsRoot(instrumentsRoot)"
                 snapPixels="4" snapActive="1" snapShown="0" overlayOpacity="0.330"
                 fixedSize="0" initialWidth="600" initialHeight="400">
  <METHODS>
    <METHOD name="handleCommandMessage (int commandId)"/>
  </METHODS>
  <BACKGROUND backgroundColour="0"/>
  <JUCERCOMP name="" id="9bde1b4dd587d5fb" memberName="skew" virtualName=""
             explicitFocusOrder="0" pos="0R 0 64 0M" posRelativeX="981ceff5817d7b34"
             sourceFile="../../Themes/SeparatorVerticalSkew.cpp" constructorParams=""/>
  <JUCERCOMP name="" id="981ceff5817d7b34" memberName="backgroundA" virtualName=""
             explicitFocusOrder="0" pos="0 0 510M 0M" sourceFile="../../Themes/PanelBackgroundA.cpp"
             constructorParams=""/>
  <JUCERCOMP name="" id="9e61167b79cef28c" memberName="backgroundB" virtualName=""
             explicitFocusOrder="0" pos="0Rr 0 446 0M" posRelativeW="4ac6bf71d1e1d84f"
             sourceFile="../../Themes/PanelBackgroundB.cpp" constructorParams=""/>
  <JUCERCOMP name="" id="d37f5d299f347b6c" memberName="pluginsList" virtualName=""
             explicitFocusOrder="0" pos="14 10 16M 20M" posRelativeW="981ceff5817d7b34"
             sourceFile="AudioPluginsListComponent.cpp" constructorParams="pluginScanner, instrumentsRoot"/>
  <JUCERCOMP name="" id="23f9d3ba9d40a668" memberName="instrumentsList" virtualName=""
             explicitFocusOrder="0" pos="14 10 28M 20M" posRelativeX="9e61167b79cef28c"
             posRelativeW="9e61167b79cef28c" sourceFile="InstrumentsListComponent.cpp"
             constructorParams="pluginScanner, instrumentsRoot"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif
