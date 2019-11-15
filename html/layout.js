// base configuration for golden layout
let config = {
    settings: {
        reorderEnabled: true,
    },
    content: [] // users can start building out their own layout immediately
};


//User can save their layout to local storage
let msgLayout;
let savedState = localStorage.getItem( 'savedState' ); // check local storage and use saved state if it exists
let editState = localStorage.getItem( 'editState' ); // check local storage and use saved state if it exists

// Sets a new Golden Layout instance, using config and attaching to the target container
// config argument is required. if no target is provided, golden layout
// will take over the document.body
if( savedState !== null && savedState !== 'undefined' ) {
    msgLayout = new GoldenLayout( JSON.parse( savedState ), $('#layout_container') );
} else {
    msgLayout = new window.GoldenLayout( config, $('#layout_container'));
}

// Golden Layout needs a component variable
var msgSelectorComponent = function( container, state ) {

    //check for local storage and
    if( !typeof window.localStorage ) {
        container.getElement()
                 .append('<h2 class="err">Your browser doesn\'t support \
                 local storage, which is necessary to save your changes.</h2>');
        return;
    }

    let componentObj = new MsgSelector(handler = state.handler, selection = state.selection);
    container.getElement().append(componentObj);

    componentObj.addEventListener('settingsChanged', function(e){
        container.setState({
            handler: state.handler,
            selection: e.detail
        })
    })

    container.on('resize', function(e){
        componentObj.resize()
    })

}

// Register components with golden layout
msgLayout.registerComponent( 'msgSelector', msgSelectorComponent);
// END register components

// Save to local storage, called by layout change and save button
function saveState(reload) {
    const state = JSON.stringify( msgLayout.toConfig() );
    localStorage.setItem( 'savedState', state );
    if(reload){
        location.reload();
    }
}

//initializing our layout
msgLayout.init();

// save state button
(function saveButton() {
    let element = $('#save_btn');

    element.click(function(e){
        e.preventDefault();
        saveState(false);
    });
}());

// lock/unlock button
(function layoutLock() {
    let element = $('#lock_btn');
    if(msgLayout.config.settings.reorderEnabled === false){
        element.html('Unlock');
    }

    // Any configs that we want to disable when screen is locked
    let lockableConfigs = ['reorderEnabled', 'showPopoutIcon', 'showCloseIcon'];

    element.click(function(){
        if(msgLayout.config.settings.reorderEnabled === true){
            for(let key in msgLayout.config.settings){
                if(lockableConfigs.includes(key)){
                    msgLayout.config.settings[key] = false;
                }
            }
        } else {
            for(let key in msgLayout.config.settings){
                if(lockableConfigs.includes(key)){
                    msgLayout.config.settings[key] = true;
                }
            }
        }
        saveState(true);
    });
}());


// Add Buttons
function addMenuItem( container, text, title, componentType, handler, selection ) {
    // if(typeof(selection) === 'undefined' || selection === '') {
    //     let selection = 'fsw.msp.hawk.TlmAck';
    // }
    const element = $( '<button class="btn btn-success" style="margin: 0 5px;">' + text + '</button>' );

    $( container ).append( element );

    const newItemConfig = {
        type: 'component',
        componentName: componentType,
        title: title,
        componentState: {
            handler: handler,
            selection: selection
        }
    };

    msgLayout.createDragSource( element, newItemConfig );
};

function createMenu(container){
    const menu = $(container);
    const directions = '<span style="display: inline-block; font-size 1.5em;">Drag items to add: </span>';
    menu.append(directions);

    addMenuItem( container, '+ View a packet', 'Packet Viewer', 'msgSelector', 'msgtools-msgrx', '');
    addMenuItem( container, '+ Send a command', 'Command Sender', 'msgSelector', 'msgtools-msgtx', '');
    addMenuItem( container, '+ Plot a message', 'Message Plot', 'msgSelector', 'msgtools-msgplot', '');
}

// Only include the add buttons if the layout is unlocked
if(msgLayout.config.settings.reorderEnabled == true ){
    createMenu('#menu_container');
}

$(window).resize(function () {
msgLayout.updateSize($(window).width(), $(window).height());
});