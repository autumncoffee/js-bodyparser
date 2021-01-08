const LIB = require('bindings')('ac_bodyparser');

function Chunk(ptr) {
    const self = this;

    self.content = function() {
        const content = LIB.GetChunkContent(ptr);

        self.content = function() {
            return content;
        };

        return content;
    };

    const headers = {};

    self.get = function(header) {
        const values = self.getAll(header);

        if (values.length == 0) {
            return null;
        }

        return values[0];
    };

    self.getAll = function(header) {
        header = header.toLowerCase();

        if (header in headers) {
            return headers[header];
        }

        headers[header] = LIB.GetChunkHeaders(ptr, header);

        return headers[header];
    };
}

module.exports = function(boundary, body) {
    const self = this;
    const ptr = LIB.ParseBody(boundary, body);

    self.delete = function() {
        LIB.Delete(ptr);

        self.delete = function() {};
    };

    const chunks = {};

    self.chunk = function(name) {
        if (name in chunks) {
            return chunks[name];
        }

        const chunkPtr = LIB.GetChunk(ptr, name);

        if (chunkPtr == 0) {
            chunks[name] = null;

        } else {
            chunks[name] = new Chunk(chunkPtr);
        }

        return chunks[name];
    };
};
