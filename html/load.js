function connectToServer() {
    var client = new msgtools.MessageClient('WebGuiDemo');
    var options = new Map();
    options.set('server', location.hostname);
    // commenting out for development, uncomment to connect
    client.connect(options);
}
console.log('start');
msgtools.load('../obj/CodeGenerator/Javascript/', ['BandwidthTest',
                                                   'Debug',
                                                   'Taxonomy.Canidae.Canis',
                                                   'Taxonomy.Canidae.Dog',
                                                   'Taxonomy.Canidae.Fox',
                                                   'Taxonomy.Canidae.Vulpes',
                                                   'Taxonomy.Canidae.Canidae',
                                                   'Taxonomy.Canidae.AFox',
                                                   'Taxonomy.Canidae.Wolf',
                                                   'TestCase3',
                                                   'TestCaseConvert',
                                                   'TestCase4',
                                                   'TestCase1',
                                                   'TestCase2',
                                                 ]).then(() => {connectToServer()});
console.log('end');
