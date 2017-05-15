'use strict';

if (typeof global.gc !== 'function') {
   throw new Error('Tests require --expose-gc flag.')
}

let testModules = [
   'arraybuffer',
   'asyncworker',
   'buffer',
   'error',
   'external',
   'function',
   'name',
];

testModules.forEach(name => {
   try {
      require('./' + name);
   }
   catch (e) {
      console.error(e);
      process.exitCode = 1;
   }
});
