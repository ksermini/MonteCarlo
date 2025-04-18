# Remove existing vcpkg directory if it exists
if (Test-Path C:\vcpkg) {
    Remove-Item -Path C:\vcpkg -Recurse -Force
}

# Clone vcpkg repository
git clone https://github.com/Microsoft/vcpkg.git C:\vcpkg

# Navigate to vcpkg directory
Set-Location C:\vcpkg

# Run bootstrap script
.\bootstrap-vcpkg.bat

# Integrate vcpkg with Visual Studio
.\vcpkg integrate install

# Install required packages
.\vcpkg install nlohmann-json:x64-windows
.\vcpkg install catch2:x64-windows
.\vcpkg install spdlog:x64-windows 