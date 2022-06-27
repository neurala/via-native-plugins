namespace neurala::dotnet {
    void initialize();
}

namespace neurala::dotnet::result_output {
    void invokeResultOutput(const char* metadata, const void* imageBytes, int width, int height);
}

namespace neurala::dotnet::video_source {
    void getMetadata(int& width, int& height);

    void moveNextFrame(int& status);

    void getFrame(void* buffer);

    void execute(const char* action);
}
