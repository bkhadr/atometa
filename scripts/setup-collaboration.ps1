# Atometa - Collaboration Setup Script
# Run this after cloning the repository for the first time

Write-Host "========================================" -ForegroundColor Cyan
Write-Host "  Atometa - Collaboration Setup" -ForegroundColor Cyan
Write-Host "  Setting up your development environment" -ForegroundColor Cyan
Write-Host "========================================" -ForegroundColor Cyan
Write-Host ""

# Check Git installation
Write-Host "[1/6] Checking Git installation..." -ForegroundColor Yellow
if (!(Get-Command git -ErrorAction SilentlyContinue)) {
    Write-Host "ERROR: Git is not installed!" -ForegroundColor Red
    Write-Host "Please install Git from https://git-scm.com" -ForegroundColor Red
    exit 1
}
Write-Host "Git found: $(git --version)" -ForegroundColor Green
Write-Host ""

# Configure Git user (if not already configured)
Write-Host "[2/6] Configuring Git user..." -ForegroundColor Yellow
$gitName = git config user.name
$gitEmail = git config user.email

if ([string]::IsNullOrEmpty($gitName)) {
    $userName = Read-Host "Enter your name for Git commits"
    git config --global user.name "$userName"
    Write-Host "Git name set to: $userName" -ForegroundColor Green
} else {
    Write-Host "Git name already configured: $gitName" -ForegroundColor Green
}

if ([string]::IsNullOrEmpty($gitEmail)) {
    $userEmail = Read-Host "Enter your email for Git commits"
    git config --global user.email "$userEmail"
    Write-Host "Git email set to: $userEmail" -ForegroundColor Green
} else {
    Write-Host "Git email already configured: $gitEmail" -ForegroundColor Green
}
Write-Host ""

# Set up develop branch
Write-Host "[3/6] Setting up branches..." -ForegroundColor Yellow
git fetch origin
$currentBranch = git rev-parse --abbrev-ref HEAD

if ($currentBranch -eq "main") {
    Write-Host "Creating and checking out develop branch..." -ForegroundColor Cyan
    git checkout -b develop origin/develop 2>$null
    if ($LASTEXITCODE -ne 0) {
        git checkout -b develop
        Write-Host "Created new develop branch" -ForegroundColor Green
    } else {
        Write-Host "Checked out existing develop branch" -ForegroundColor Green
    }
} else {
    Write-Host "Current branch: $currentBranch" -ForegroundColor Green
}
Write-Host ""

# Check vcpkg installation
Write-Host "[4/6] Checking vcpkg installation..." -ForegroundColor Yellow
if (!(Get-Command vcpkg -ErrorAction SilentlyContinue)) {
    Write-Host "WARNING: vcpkg not found in PATH" -ForegroundColor Yellow
    Write-Host "You'll need to install dependencies manually" -ForegroundColor Yellow
    Write-Host ""
    Write-Host "Install vcpkg:" -ForegroundColor Cyan
    Write-Host "  1. git clone https://github.com/Microsoft/vcpkg.git C:\vcpkg" -ForegroundColor White
    Write-Host "  2. cd C:\vcpkg" -ForegroundColor White
    Write-Host "  3. .\bootstrap-vcpkg.bat" -ForegroundColor White
    Write-Host "  4. .\vcpkg integrate install" -ForegroundColor White
    Write-Host ""
} else {
    Write-Host "vcpkg found" -ForegroundColor Green
}
Write-Host ""

# Create your first branch
Write-Host "[5/6] Ready to create your first feature branch?" -ForegroundColor Yellow
$createBranch = Read-Host "Create a feature branch now? (y/n)"

if ($createBranch -eq "y" -or $createBranch -eq "Y") {
    $branchName = Read-Host "Enter branch name (e.g., feature/my-feature)"
    git checkout -b $branchName
    Write-Host "Created and checked out branch: $branchName" -ForegroundColor Green
    Write-Host ""
    Write-Host "Next steps:" -ForegroundColor Cyan
    Write-Host "  1. Make your changes" -ForegroundColor White
    Write-Host "  2. git add ." -ForegroundColor White
    Write-Host "  3. git commit -m 'feat: Your feature description'" -ForegroundColor White
    Write-Host "  4. git push origin $branchName" -ForegroundColor White
    Write-Host "  5. Create Pull Request on GitHub" -ForegroundColor White
} else {
    Write-Host "Skipped branch creation" -ForegroundColor Gray
}
Write-Host ""

# Display useful information
Write-Host "[6/6] Setup Summary" -ForegroundColor Yellow
Write-Host "==================" -ForegroundColor Yellow
Write-Host ""
Write-Host "Git configured:" -ForegroundColor Cyan
Write-Host "  Name:  $(git config user.name)" -ForegroundColor White
Write-Host "  Email: $(git config user.email)" -ForegroundColor White
Write-Host ""
Write-Host "Current branch: $(git rev-parse --abbrev-ref HEAD)" -ForegroundColor Cyan
Write-Host ""
Write-Host "Important files to read:" -ForegroundColor Cyan
Write-Host "  - COLLABORATION.md  (Collaboration guide)" -ForegroundColor White
Write-Host "  - CONTRIBUTING.md   (How to contribute)" -ForegroundColor White
Write-Host "  - README.md         (Project overview)" -ForegroundColor White
Write-Host ""
Write-Host "Next steps:" -ForegroundColor Cyan
Write-Host "  1. Read CONTRIBUTING.md" -ForegroundColor White
Write-Host "  2. Build the project: .\build-ninja.bat" -ForegroundColor White
Write-Host "  3. Run tests: .\build\bin\AtometaTests.exe" -ForegroundColor White
Write-Host "  4. Start coding!" -ForegroundColor White
Write-Host ""
Write-Host "========================================" -ForegroundColor Green
Write-Host "  Setup Complete! Happy Coding!" -ForegroundColor Green
Write-Host "========================================" -ForegroundColor Green
Write-Host ""