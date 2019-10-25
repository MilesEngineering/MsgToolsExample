// base configuration for golden layout
var config = {
    content: [{
        type: 'row',
        content:[{
            type: 'component',
            componentName: 'msgSelector',
            componentState: { handler: 'msgtools-msgtx', filter: 'tlm'}
        },{
            type: 'column',
            content:[{
                type: 'component',
                componentName: 'msgSelector',
                componentState: { handler: 'msgtools-msgrx'}
            },{
                type: 'component',
                componentName: 'msgTree',
                componentState: { handler: 'msgtools-msgrx'}
            }]
        }]
    }]
};

// Sets a new Golden Layout instance, using config and attaching to the target container
// config argumnet is required. if no target is provided, golden layout
// will take over the document.body
var msgLayout,
    savedState = localStorage.getItem( 'savedState' ); // check local storage and use saved state if it exists

if( savedState !== null ) {
    if (savedState === 'undefined') {
        msgLayout = new window.GoldenLayout( config, $('#layout_container'));
    } else {
        msgLayout = new GoldenLayout( JSON.parse( savedState ), $('#layout_container') );
    }
} else {
    msgLayout = new window.GoldenLayout( config, $('#layout_container'));
}


// Registering components for golden layout
msgLayout.registerComponent( 'msgSelector', function( container, state ){
    container.getElement().html('<div><h3>Type: '
                                + state.handler
                                + '</h3><msgtools-msgselector handler="'
                                + state.handler
                                + '"></msgtools-msgselector></div>');
});

msgLayout.registerComponent( 'msgTree', function( container, state ){
    container.getElement().html('<div><h3>Type: '
                                + state.handler
                                + '</h3><msgtools-msgtree handler='
                                + state.handler
                                + '></msgtools-msgtree></div>');
});
// END registering components

// Store state in local storage
msgLayout.on( 'stateChanged', function(){
    var state = JSON.stringify( msgLayout.toConfig() );
    localStorage.setItem( 'savedState', state );
    console.log(state);
});

//initializing our layout
msgLayout.init();

// Add Buttons
function addMenuItem( text, componentType, handler ) {

    var element = $( '<button>' + text + '</button>' );

    $( '#menuContainer' ).append( element );

    var newItemConfig = {
        type: 'component',
        componentName: componentType,
        componentState: { handler: handler }
    };

    msgLayout.createDragSource( element, newItemConfig );
};

addMenuItem( 'Add msgSelector receive', 'msgSelector', 'msgtools-msgrx' );
addMenuItem( 'Add msgTree receive', 'msgTree', 'msgtools-msgrx' );
addMenuItem( 'Add msgSelector transmit', 'msgSelector', 'msgtools-msgtx' );
addMenuItem( 'Add msgTree transmit', 'msgTree', 'msgtools-msgtx' );
