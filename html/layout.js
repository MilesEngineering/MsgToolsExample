// base configuration for golden layout
var config = {
    content: [{
        type: 'row',
        content:[{
            type: 'component',
            componentName: 'msgComponent',
            componentState: { handler: 'msgtools-msgtx' }
        },{
            type: 'column',
            content:[{
                type: 'component',
                componentName: 'msgComponent',
                componentState: { handler: 'msgtools-msgrx' }
            },{
                type: 'component',
                componentName: 'test',
                componentState: { handler: 'msgtools-msgrx' }
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
    msgLayout = new GoldenLayout( JSON.parse( savedState ), $('#layout_container') );
} else {
    msgLayout = new window.GoldenLayout( config, $('#layout_container'));
}


// Registering components for golden layout
msgLayout.registerComponent( 'msgComponent', function( container, state ){
    container.getElement().html('<div><msgtools-msgselector handler="'
                                + state.handler
                                + '"></msgtools-msgselector></div>');
});

msgLayout.registerComponent( 'test', function( container, state ){
    container.getElement().html('<div>What what</div>');
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
var addMenuItem = function( text, handler ) {

    var element = $( '<button>' + text + '</button>' );

    $( '#menuContainer' ).append( element );

    var newItemConfig = {
        type: 'component',
        componentName: 'msgComponent',
        componentState: { handler: handler }
    };

    msgLayout.createDragSource( element, newItemConfig );
};

addMenuItem( 'Add a new msgtools-msgrx', 'msgtools-msgrx' );
addMenuItem( 'Add a new msgtools-msgtx', 'msgtools-msgtx' );
