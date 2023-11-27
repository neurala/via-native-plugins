#include <BajoranVideoSource.h>

#include <holoscan/holoscan.hpp>

#include <neurala/plugin/PluginArguments.h>
#include <neurala/video/VideoSource.h>

#include <cstdint>
#include <memory>

namespace {
    // The format of the frame data that will come from the Bajoran board is
    // not yet known. This is just a placeholder type just for illustration.
    struct ManagedBuffer {
        std::shared_ptr<uint8_t> data;

        uint32_t width;
        uint32_t height;
    };

    // In GStreamer parlance, this is the element that collects the frame data
    // from whatever element is supposed to pipe them to us.
    class NeuralaSinkOperator final : public holoscan::Operator {
      private:
        holoscan::expected<ManagedBuffer, holoscan::RuntimeError> lastResult;

      public:
        const auto & getLastResult() const noexcept {
            return lastResult;
        }

        virtual void setup(holoscan::OperatorSpec & spec) final override;
        virtual void compute(holoscan::InputContext & input,
                             holoscan::OutputContext & output,
                             holoscan::ExecutionContext & execution) final override;
    };

    void NeuralaSinkOperator::setup(holoscan::OperatorSpec & spec) {
        // Declare an input pad called "in".
        spec.input<ManagedBuffer>("in");
    }

    void NeuralaSinkOperator::compute(holoscan::InputContext & input,
                                      holoscan::OutputContext & output,
                                      holoscan::ExecutionContext & execution)
    {
        lastResult = input.receive<ManagedBuffer>("in");
    }
}

namespace neurala {
    // In GStreamer parlance, this is the whole pipeline.
    class NeuralaBajoranApplication final : public holoscan::Application {
      private:
        NeuralaSinkOperator & sinkOperator;

      public:
        const auto & getLastResult() const noexcept {
            return sinkOperator.getLastResult();
        }

        virtual void compose() final override;
    };

    void NeuralaBajoranApplication::compose() {
        // TODO
    }

    BajoranVideoSource::BajoranVideoSource() : m_implementation(std::make_unique<NeuralaBajoranApplication>()) {
        //
    }

    dto::ImageMetadata BajoranVideoSource::metadata() const noexcept {
        const auto & result = m_implementation->getLastResult().operator*();
        return dto::ImageMetadata("uint8", result.width, result.height, "RGB", "interleaved", "topLeft");
    }

    std::error_code BajoranVideoSource::nextFrame() noexcept {
        // TODO
    }

    dto::ImageView BajoranVideoSource::frame() const noexcept {
        const auto & result = m_implementation->getLastResult().operator*();
        const auto data = result.data.get();
        return dto::ImageView(dto::ImageMetadata("uint8",
                                                 result.width,
                                                 result.height,
                                                 "RGB",
                                                 "interleaved",
                                                 "topLeft"),
                              data);
    }

    dto::ImageView BajoranVideoSource::frame(std::byte * buffer, std::size_t size) const noexcept {
        const auto & result = m_implementation->getLastResult().operator*();
        memcpy(buffer, result.data.get(), size);
        return dto::ImageView(metadata(), buffer);
    }

    std::error_code BajoranVideoSource::execute(const std::string &) noexcept {
        //
    }
}
