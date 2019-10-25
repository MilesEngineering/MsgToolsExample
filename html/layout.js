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
                componentName: 'msgComponent',
                componentState: { handler: 'msgtools-msgrx' }
            }]
        }]
    }]
};


// Sets a new Golden Layout instance, using config and attaching to the target container
// config argumnet is required. if no target is provided, golden layout
// will take over the document.body
var msgLayout = new window.GoldenLayout(config, $('#layout_container'));

// Registering components for golden layout
msgLayout.registerComponent( 'msgComponent', function( container, state ){
    container.getElement().html('<div><msgtools-msgselector handler="'
                                + state.handler
                                + '"></msgtools-msgselector></div>');
});


//initializing our layout
msgLayout.init();

// var addMenuItem = function( text ) {
//     var element = $( '<li>' + text + '</li>' );
//     $( 'nav' ).append( element );
//
//     //insertion code will go here
// };
//
// addMenuItem( 'User added component A' );
// addMenuItem( 'User added component B' );
