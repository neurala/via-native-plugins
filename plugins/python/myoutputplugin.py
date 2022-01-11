def munge(metadata: str):
    return metadata.replace("\n", "")

def action1(metadata: str):
    print(f"action1, metadata={munge(metadata)}")

def action2(metadata: str):
    print(f"action2, metadata={munge(metadata)}")

def outputFrame(metadata: str):
    print(f"outputFrame, metadata={munge(metadata)}")
