USE_LOCAL_STORAGE = 0

class MainLayout {
    constructor() {
        // base configuration for golden layout
        var config = {
            settings: {
            },
            content: [] // users can start building out their own layout immediately
        };

        var currentPath = 'http://' + window.location.hostname + ":" + window.location.port + '/';

        this.settingsStorage = new SettingsStorage(currentPath);

        this.settingsFilename = localStorage.getItem( 'settingsFilename');
        var savedState;
        if(USE_LOCAL_STORAGE) {
            // check local storage
            savedState = localStorage.getItem( 'savedState.'+this.settingsFilename );
        } else {
            savedState = this.settingsStorage.load(this.settingsFilename);
        }
        // use saved state if it exists
        if( savedState !== null && savedState !== undefined && savedState != "") {
            config = JSON.parse( savedState );
        }

        // Sets a new Golden Layout instance, using config and attaching to the target container
        // config argument is required. if no target is provided, golden layout
        // will take over the document.body
        this.msgLayout = new GoldenLayout( config, $('#layout_container') );
        this.msgLayout.registerComponent( 'msgSelector', msgSelectorComponent );
        this.msgLayout.init();

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
                    // save and reload, otherwise settings in golden layout
                    // don't take effect except on new containers.
                    that.saveConfig(that.settingsGui.settingsName);
                    location.reload();
                };
            }
        }

        this.createMenu($('#toolbar_container'), $('#menu_container'));

        this.stateClean(true);
    }
    stateClean(clean) {
        this.settingsGui.stateClean(clean);
    }

    // Save to local storage, called by save button
    saveConfig(filename) {
        console.log('save '+filename);
        const state = JSON.stringify( this.msgLayout.toConfig() );
        if(USE_LOCAL_STORAGE) {
            localStorage.setItem( 'savedState.'+filename, state );
        } else {
            this.settingsStorage.save(filename, state);
        }
        this.stateClean(true);
    }

    loadConfig(filename) {
        console.log('load '+filename);
        if(this.settingsFilename != filename) {
            localStorage.setItem('settingsFilename', filename);
            location.reload();
        }
    }
    deleteConfig(filename) {
        console.log('delete '+filename);
        if(USE_LOCAL_STORAGE) {
            localStorage.deleteItem( 'savedState.'+filename );
        } else {
            this.settingsStorage.rm(filename);
        }
        location.reload();
    }

    setEditable(editable) {
        console.log(editable);
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
        // don't change editable state of all widgets.  they have their own buttons for that.
        // also, currently lock/unlock causes a reload, so doing the below would have no effect.
        if(0) {
            // iterate over containers, set each one's widget's editable state
            function setChildrenEditable(container, editable, spaces="") {
                if(container.isComponent) {
                    var item = container.element[0].firstChild.firstChild;
                    item.setEditable(editable);
                } else {
                    for(var i = 0; i < container.contentItems.length; i++ ) {
                        setChildrenEditable(container.contentItems[i], editable, spaces+" ");
                    }
                }
            }
            setChildrenEditable(this.msgLayout.root, editable);
        }
    }

    // Add Buttons
    addMenuItem( container, text, title, componentType, handler ) {
        const element = $( '<button class="btn btn-success" style="margin: 0 5px;">' + text + '</button>' );

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
        if(USE_LOCAL_STORAGE) {
            return {entries:[{name:'a'},
                             {name:'b',
                              entries: [{name:'1'}, {name:'2'}]
                             }
                            ]
                   };
        } else {
            return this.settingsStorage.list();
        }
    }

    createMenu(toolBar, menu){
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
        toolBar.append( this.settingsGui.saveButton );
        menu.append( this.settingsGui.saveAsButton );
        menu.append( this.settingsGui.newFilename );
        menu.append( this.settingsGui.chooseSettingsDropdown );
        menu.append( this.settingsGui.deleteButton );
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
