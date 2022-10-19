//* render_graph
//      includes the attachment, subpass and renderpass abstraction and how they are connected
#pragma once

#include "bidirectional_map.h"
#include "strong_types.h"
#include "type_name.h"
#include <unordered_set>

namespace fresa::graphics
{
    // ···············
    // · DEFINITIONS ·
    // ···············

    //: strong types for resource ids
    using AttachmentID = strong::Type<ui32, decltype([]{}), strong::Regular, strong::Hashable, strong::Formattable>;
    using RenderpassID = strong::Type<ui32, decltype([]{}), strong::Regular, strong::Hashable, strong::Formattable>;

    // ·············
    // · DATATYPES ·
    // ·············

    namespace rendergraph
    {
        //: renderpass object
        struct Renderpass {
            RenderpassID id;
        };
    }

    //: render graph
    //      contains lists containing all attachments, subpasses and renderpasses
    //      also has bidirectional maps to connect all of them
    template <std::size_t R, std::size_t A> requires (R >= 0 && A >= 0)
    struct RenderGraph {
        static constexpr std::size_t RenderpassCount = R;
        static constexpr std::size_t AttachmentCount = A;
        constexpr RenderGraph() = default;

        //: move constructor from one less attachment / renderpass
        constexpr RenderGraph(const RenderGraph<R, A-1>&& other) : renderpasses{other.renderpasses} {
            //!
        }
        constexpr RenderGraph(const RenderGraph<R-1, A>&& other, rendergraph::Renderpass rp) : attachments{other.attachments} {
            for (std::size_t i = 0; i < R-1; ++i)
                renderpasses.at(i) = other.renderpasses.at(i);
            renderpasses.at(R-1) = rp.id;
        }

        //: lists of attachments and renderpasses
        std::array<AttachmentID, AttachmentCount> attachments;
        std::array<RenderpassID, RenderpassCount> renderpasses;

        //: check if renderpass is already in the list
        constexpr bool has_renderpass(const RenderpassID& id) const {
            return std::find(renderpasses.begin(), renderpasses.end(), id) != renderpasses.end();
        }
    };

    namespace concepts
    {
        //: rendergraph concept
        template <typename T>
        concept RenderGraph = requires (T t) {
            { t.attachments } -> std::convertible_to<std::array<AttachmentID, std::decay_t<T>::AttachmentCount>>;
            { t.renderpasses } -> std::convertible_to<std::array<RenderpassID, std::decay_t<T>::RenderpassCount>>;
        };
    }

    namespace rendergraph
    {
        //: add renderpass to render graph
        template <concepts::RenderGraph RG>
        constexpr auto addPass(const RG& graph, Renderpass rp) {
            using RT = std::decay_t<RG>;
            return RenderGraph<RT::RenderpassCount+1, RT::AttachmentCount>{std::move(graph), rp};
        }
    }

    //: user defined literals for creating renderpass objects
    constexpr auto operator""_renderpass (const char* str, size_t len) {
        return rendergraph::Renderpass{.id = hash_fnv1a(str_view{str, len})};
    }

    //: pipe operator for adding renderpasses to render graph
    /*template <concepts::RenderGraph RG>
    constexpr auto operator|(RG&& graph, renderpass::Renderpass rp) {
        return renderpass::addPass(std::forward<RG>(graph), rp);
    }*/
}