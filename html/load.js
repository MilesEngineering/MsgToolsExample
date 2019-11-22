function connectToServer() {
    var client = new msgtools.MessageClient('WebGuiDemo');
    var options = new Map();
    options.set('server', location.hostname);
    // commenting out for development, uncomment to connect
    client.connect(options);
}
console.log('start');
msgtools.load('../obj/CodeGenerator/Javascript/', ['TextCommand',
                                                   'TestCase3',
                                                   'BandwidthTest',
                                                   'TextResponse',
                                                   'TestCase4',
                                                   'Printf',
                                                   'TestCase1',
                                                   'TestCase2',
                                                   'Taxonomy.Canidae.Wolf',
                                                   'Taxonomy.Canidae.Canis',
                                                   'Taxonomy.Canidae.Vulpes',
                                                   'Taxonomy.Canidae.Dog',
                                                   'Taxonomy.Canidae.AFox',
                                                   'Taxonomy.Canidae.Fox',
                                                   'Taxonomy.Canidae.Canidae'
                                                 ]).then(() => {connectToServer()});
console.log('end');
