const ACBodyParser = require('./');

const boundary = '9dfe29a1d8d5ccaec52e1ca1589f0e02';
const partName = 'test_part';
const headerName = 'X-Custom-Header';
const headerValue = 'header+value';
const data = `
Lorem ipsum dolor sit amet, consectetur adipiscing
elit, sed do eiusmod tempor incididunt ut labore et
dolore magna aliqua. Ut enim ad minim veniam, quis
nostrud exercitation ullamco laboris nisi ut aliquip
ex ea commodo consequat. Duis aute irure dolor in
reprehenderit in voluptate velit esse cillum dolore
eu fugiat nulla pariatur. Excepteur sint occaecat
cupidatat non proident, sunt in culpa qui officia
deserunt mollit anim id est laborum.
`;

const rawBody = Buffer.from(`--${boundary}\r\nContent-Disposition: form-data; name="${partName}"; filename="${partName}"\r\n${headerName}: ${headerValue}\r\n\r\n${data}\r\n--${boundary}--\r\n`);

function assert(cond, msg) {
  if (!cond) {
    throw new Error(msg);
  }
}

const body = new ACBodyParser(boundary, rawBody);
const chunk = body.chunk(`"${partName}"`);

assert(!!chunk, `Chunk ${partName} must exist`);

assert(chunk.content().toString() == data, 'Chunk content must match expectations');

assert(chunk.get(headerName) == headerValue, `Value of header ${headerName} must match expectations`);

body.delete(); // This frees memory!

console.log('OK');
