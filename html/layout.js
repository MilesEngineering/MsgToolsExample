class MainLayout {
    constructor() {
        var currentPath = 'http://' + window.location.hostname + ":" + window.location.port + '/';
        this.settingsStorage = new SettingsStorage(currentPath);

        this.loadLayout();

        this.settingsGui = undefined;

        // lock/unlock button
        {
            let lockBtn = document.getElementById('lock_btn');
            if(lockBtn != undefined) {
                if(this.msgLayout.config.settings['showCloseIcon'] != true) {
                    lockBtn.textContent = 'Unlock';
                }
                var that = this;
                lockBtn.onclick = function(){
                    let editable = (this.textContent == 'Unlock');
                    that.setEditable(editable);
                };
            }
        }

        this.createMenu($('#tab_toggle'), $('#menu_toggle'), $('#config_name'));

        this.stateClean(true);
    }
    // load the layout configuration from persistent storage
    loadLayout() {
        // base configuration for golden layout
        var config = {
            settings: {
            },
            content: [] // users can start building out their own layout immediately
        };

        this.settingsFilename = localStorage.getItem( 'settingsFilename');
        const savedState = this.settingsStorage.load(this.settingsFilename);
        // use saved state if it exists
        if( savedState !== null && savedState !== undefined && savedState != "") {
            config = JSON.parse( savedState );
        }
        
        // destroy the old golden layout, if there was one.
        if(this.msgLayout !== undefined) {
            this.msgLayout.destroy();
        }

        // Sets a new Golden Layout instance, using config and attaching to the target container
        // config argument is required. if no target is provided, golden layout
        // will take over the document.body
        this.msgLayout = new GoldenLayout( config, $('#layout_container') );
        this.msgLayout.registerComponent( 'msgSelector', msgSelectorComponent );
        this.msgLayout.init();
    }
    
    // mark the state as clean or dirty, to enable/disable the save button, as a cue
    // to the user for if their latest changes are saved or not.
    stateClean(clean) {
        this.settingsGui.stateClean(clean);
    }

    // Save to local storage, called by save button
    saveConfig(filename) {
        console.log('save '+filename);
        const state = JSON.stringify( this.msgLayout.toConfig() );
        this.settingsStorage.save(filename, state);
        this.stateClean(true);
    }

    loadConfig(filename) {
        //console.log('load '+filename);
        if(this.settingsFilename != filename) {
            localStorage.setItem('settingsFilename', filename);
            this.loadLayout();
        }
    }
    deleteConfig(filename) {
        console.log('delete '+filename);
        this.settingsStorage.rm(filename);
        this.loadLayout();
    }

    // Set the layout as editable or not.  this involves
    // changing goldenlayout settings, destroying the layout,
    // and recreating it.  Otherwise the settings change would
    // only apply to newly created containers.
    setEditable(editable) {
        // Any configs that we want to disable when screen is locked
        let lockableConfigs = ['reorderEnabled', 'showPopoutIcon', 'showCloseIcon', 'hasHeaders', 'showMaximiseIcon'];
        // how to disable splitters?
        // https://github.com/golden-layout/golden-layout/issues/351
        // .lm_splitter { visibility: hidden; pointer-events: none; }
        let lockBtn = document.getElementById('lock_btn');
        if(editable){
            lockBtn.textContent = 'Lock';
        } else {
            lockBtn.textContent = 'Unlock';
        }
        this.msgLayout.config.settings.reorderEnabled = true;
        for(let key in this.msgLayout.config.settings){
            if(lockableConfigs.includes(key)){
                this.msgLayout.config.settings[key] = editable;
            }
        }
        
        // make a copy of the settings that we can keep after we destory the layout
        const config = JSON.parse( JSON.stringify( this.msgLayout.toConfig() ) );

        // destroy the layout
        this.msgLayout.destroy();
        
        // make a new one using the copy of the settings
        this.msgLayout = new GoldenLayout( config, $('#layout_container') );
        this.msgLayout.registerComponent( 'msgSelector', msgSelectorComponent );
        this.msgLayout.init();
        
        // mark as dirty, so the user knows they can save it
        this.stateClean(false);
    }

    // Add Buttons
    addMenuItem( container, text, title, componentType, handler ) {
        const element = $( '<button class="btn btn-success">' + text + '</button>' );

        container.append( element );

        const newItemConfig = {
            type: 'component',
            componentName: componentType,
            title: title,
            componentState: {
                handler: handler,
                selection: undefined
            }
        };

        this.msgLayout.createDragSource( element, newItemConfig );
    }

    getSettingsChoices() {
        return this.settingsStorage.list();
    }

    createMenu(toolBar, menu, titlebar){
        const directions = '<span style="display: inline-block; font-size 1.5em;">Drag items to add: </span>';
        toolBar.append(directions);

        this.addMenuItem( toolBar, '+ Rx Row', 'Packet Viewer', 'msgSelector', 'MsgRxRow');
        this.addMenuItem( toolBar, '+ Rx Column', 'Packet Viewer', 'msgSelector', 'MsgRxColumn');
        this.addMenuItem( toolBar, '+ Tx Row', 'Command Sender', 'msgSelector', 'MsgTxRow');
        this.addMenuItem( toolBar, '+ Tx Column', 'Command Sender', 'msgSelector', 'MsgTxColumn');
        this.addMenuItem( toolBar, '+ Plot a message', 'Message Plot', 'msgSelector', 'MsgPlot');

        this.settingsGui = new SettingsGui(this.settingsFilename, this.getSettingsChoices.bind(this));
        var that = this;
        this.settingsGui.addEventListener('save', function(e){
            let filename = e.detail;
            that.saveConfig(filename);
        })
        this.settingsGui.addEventListener('load', function(e){
            let filename = e.detail;
            that.loadConfig(filename);
        })
        this.settingsGui.addEventListener('delete', function(e){
            let filename = e.detail;
            that.deleteConfig(filename);
        })
        this.loadConfig(this.settingsFilename);

        titlebar.append( this.settingsGui.currentConfigHeader );

        menu.prepend( this.settingsGui.saveButton );
        const chooseFileWrapper = `<div class='filename-wrapper'>
                                     <button id='open_btn' class='btn btn-open' target='choose_file'>Open</button> \
                                     <div class='filename-form closed' id='choose_file'></div> \
                                 </div>`;
        menu.prepend(chooseFileWrapper);

        menu.find('#choose_file').append( this.settingsGui.chooseSettingsDropdown );
        menu.prepend( this.settingsGui.deleteButton );

        const filenameWrapper = `<div class='filename-wrapper'>
                                     <button id='new_btn' class='btn btn-new' target='new_file'>New</button> \
                                     <div class='filename-form closed' id='new_file'></div> \
                                 </div>`;
        menu.prepend(filenameWrapper);
        menu.find('#new_file').prepend( this.settingsGui.newFilename );
        menu.find('#new_file').append( this.settingsGui.saveAsButton );

    }

}

var mainLayout = new MainLayout();

// Golden Layout needs a component variable
function msgSelectorComponent( container, state ) {

    let componentObj = new MsgSelector(state.handler, state.selection, state);
    container.getElement().append(componentObj);

    // if it's a brand new component, mark our state as dirty so it can be saved.
    if(state.selection == undefined) {
        mainLayout.stateClean(false);
    }

    // when our component's settings change, store them in the container,
    // and mark the state as unclean.
    componentObj.addEventListener('settingsChanged', function(e){
        container.setState(e.detail);
        mainLayout.stateClean(false);
    })

    // when container changes size, tell the component about it.
    container.on('resize', function(){
        componentObj.resize(container.width, container.height);
    })

    // when container is destroyed, also destroy the component.
    container.on('destroy', function(){
        componentObj.destroy();
    })
}

$(window).resize(function () {
    let height = $('#layout_container').height();
    let width = $('#layout_container').width();
    mainLayout.msgLayout.updateSize(width, height);
});
