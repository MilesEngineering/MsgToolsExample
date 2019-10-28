// base configuration for golden layout
let config = {
    settings: {
        reorderEnabled: true,
    },
    content: [] // users can start building out their own layout immediately
};

// Sets a new Golden Layout instance, using config and attaching to the target container
// config argumnet is required. if no target is provided, golden layout
// will take over the document.body
let msgLayout;
let savedState = localStorage.getItem( 'savedState' ); // check local storage and use saved state if it exists
let editState = localStorage.getItem( 'editState' ); // check local storage and use saved state if it exists


if( savedState !== null && savedState !== 'undefined' ) {
    msgLayout = new GoldenLayout( JSON.parse( savedState ), $('#layout_container') );
} else {
    msgLayout = new window.GoldenLayout( config, $('#layout_container'));
}


// Registering components for golden layout
msgLayout.registerComponent( 'msgSelector', function( container, state ){
    container.getElement().html('<div><msgtools-msgselector handler="'
                                + state.handler
                                + '"></msgtools-msgselector></div>');
});

msgLayout.registerComponent( 'msgTree', function( container, state ){
    container.getElement().html('<div><msgtools-msgtree handler='
                                + state.handler
                                + '></msgtools-msgtree></div>');
});
// END registering components

function saveState(reload) {
    const state = JSON.stringify( msgLayout.toConfig() );
    localStorage.setItem( 'savedState', state );
    if(reload){
        location.reload();
    }
}

// Store state in local storage
msgLayout.on( 'stateChanged', function(){
    saveState(false);
});

//initializing our layout
msgLayout.init();

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
function addMenuItem( container, text, componentType, handler ) {

    const element = $( '<button class="btn btn-success" style="margin: 0 5px;">' + text + '</button>' );

    $( container ).append( element );

    var newItemConfig = {
        type: 'component',
        componentName: componentType,
        componentState: { handler: handler }
    };

    msgLayout.createDragSource( element, newItemConfig );
};

function createMenu(container){
    const menu = $(container);
    const directions = '<span style="display: inline-block; font-size 1.5em;">Drag items to add: </span>';
    menu.append(directions);

    addMenuItem( container, 'Add msgSelector receive', 'msgSelector', 'msgtools-msgrx' );
    addMenuItem( container, 'Add msgTree receive', 'msgTree', 'msgtools-msgrx' );
    addMenuItem( container, 'Add msgSelector transmit', 'msgSelector', 'msgtools-msgtx' );
    addMenuItem( container, 'Add msgTree transmit', 'msgTree', 'msgtools-msgtx' );
}

// Only include the add buttons if the layout is unlocked
if(msgLayout.config.settings.reorderEnabled == true ){
    createMenu('#menu_container');
}
