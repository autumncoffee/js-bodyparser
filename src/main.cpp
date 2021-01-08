#include <napi.h>

#include <ac-library/http/utils/multipart.hpp>
#include <ac-common/utils/string.hpp>
#include <string>
#include <vector>
#include <cstdint>

namespace NACJSBodyParser {
    using namespace NAC;

    using TBodyParts = std::vector<NHTTP::TBodyPart>;

    namespace {
        template<typename T>
        uintptr_t AsPtr(const T& arg) {
            return arg.template As<Napi::Number>().Int64Value();
        }
    }

    Napi::Value ParseBody(const Napi::CallbackInfo& info) {
        auto input = info[1].As<Napi::Buffer<uint8_t>>();

        return Napi::Number::New(info.Env(), (uintptr_t)new TBodyParts(NHTTPUtils::ParseParts(info[0].As<Napi::String>(), input.Length(), (const char*)input.Data())));
    }

    Napi::Value Delete(const Napi::CallbackInfo& info) {
        delete (TBodyParts*)AsPtr(info[0]);

        return info.Env().Undefined();
    }

    namespace {
        NHTTP::TBodyPart* FindChunk(TBodyParts& list, const std::string& name) {
            for (auto&& part : list) {
                const auto& params = part.ContentDispositionParams();
                auto it = params.find(std::string("name"));

                if (it == params.end()) {
                    continue;
                }

                if (it->second == name) {
                    return &part;
                }
            }

            return nullptr;
        }
    }

    Napi::Value GetChunk(const Napi::CallbackInfo& info) {
        return Napi::Number::New(info.Env(), (uintptr_t)FindChunk(*(TBodyParts*)AsPtr(info[0]), info[1].As<Napi::String>()));
    }

    Napi::Value GetChunkContent(const Napi::CallbackInfo& info) {
        auto& chunk = *(NHTTP::TBodyPart*)AsPtr(info[0]);

        return Napi::Buffer<uint8_t>::New(info.Env(), (uint8_t*)chunk.Content(), chunk.ContentLength());
    }

    Napi::Value GetChunkHeaders(const Napi::CallbackInfo& info) {
        auto env = info.Env();
        std::string headerName(info[1].As<Napi::String>());
        NStringUtils::ToLower(headerName);

        const auto* values = ((NHTTP::TBodyPart*)AsPtr(info[0]))->HeaderValues(headerName);

        if (values) {
            auto rv = Napi::Array::New(env, values->size());

            for (size_t i = 0; i < values->size(); ++i) {
                rv.Set(i, values->at(i));
            }

            return rv;
        }

        return Napi::Array::New(env, 0);
    }
}

Napi::Object ACJSBodyParserInit(Napi::Env env, Napi::Object exports) {
  exports.Set("ParseBody", Napi::Function::New(env, NACJSBodyParser::ParseBody));
  exports.Set("Delete", Napi::Function::New(env, NACJSBodyParser::Delete));
  exports.Set("GetChunk", Napi::Function::New(env, NACJSBodyParser::GetChunk));
  exports.Set("GetChunkContent", Napi::Function::New(env, NACJSBodyParser::GetChunkContent));
  exports.Set("GetChunkHeaders", Napi::Function::New(env, NACJSBodyParser::GetChunkHeaders));

  return exports;
}

NODE_API_MODULE(ac_bodyparser, ACJSBodyParserInit)
