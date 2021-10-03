const { gzipAsync } = require('@gfx/zopfli');
const FS = require('fs');
const path = require('path');

const SAVE_PATH = '../src';

const BUNDLE_JS = FS.readFileSync(path.resolve(__dirname, './dist/js/app.js'));
const INDEX_HTML = 
`<!DOCTYPE html>
<html lang="en">
<head>
	<meta charset='utf-8'>
	<meta name='viewport' content='width=device-width,initial-scale=1'>
	<title>WebSerial</title>
  <script data-name="BMC-Widget" async src="https://cdnjs.buymeacoffee.com/1.0.0/widget.prod.min.js" data-id="6QGVpSj" data-description="Support me on Buy me a coffee!" data-message="You can always support my work by buying me a coffee!" data-color="#FF813F" data-position="right" data-x_margin="24" data-y_margin="24"></script>
</head>
<body>
<div id="app"></div>
<script>
${BUNDLE_JS}
</script>
</body>
</html>
`;

function chunkArray(myArray, chunk_size){
  var index = 0;
  var arrayLength = myArray.length;
  var tempArray = [];
  for (index = 0; index < arrayLength; index += chunk_size) {
      myChunk = myArray.slice(index, index+chunk_size);
      // Do something if you want with the group
      tempArray.push(myChunk);
  }
  return tempArray;
}

function addLineBreaks(buffer){
  let data = '';
  let chunks = chunkArray(buffer, 30);
  chunks.forEach((chunk, index) => {
    data += chunk.join(',');
    if(index+1 !== chunks.length){
      data+=',\n';
    }
  });
  return data;
}


(async function(){
  try{
    const GZIPPED_INDEX = await gzipAsync(INDEX_HTML, { numiterations: 15 });

    const FILE = 
`
#ifndef _webserial_webapge_h
#define _webserial_webpage_h

const uint32_t WEBSERIAL_HTML_SIZE = ${GZIPPED_INDEX.length};
const uint8_t WEBSERIAL_HTML[] PROGMEM = { 
${ addLineBreaks(GZIPPED_INDEX) }
};

#endif
`;

    FS.writeFileSync(path.resolve(__dirname, SAVE_PATH+'/webserial_webpage.h'), FILE);
    console.log(`[COMPRESS.js] Compressed Bundle into webpage.h header file | Total Size: ${(GZIPPED_INDEX.length / 1024).toFixed(2) }KB`)
  }catch(err){
    return console.error(err);
  }
})();