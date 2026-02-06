# Security and Certificates

## Code Signing Certificates
Certificate files should NEVER be committed to version control. Store them securely:

### For Development:
1. Place certificates in `C:/atometa/certificates/` (outside the repo)
2. Reference them in CMake with full paths
3. Use environment variables for passwords

### Production Certificates:
- Use Azure Key Vault, AWS KMS, or HashiCorp Vault
- Use certificate thumbprints instead of files
- Implement automated signing in CI/CD pipelines

## Environment Setup
Create a `.env.local` file (gitignored):
```env
CODE_SIGN_CERT=C:/atometa/certificates/atometa.pfx
CODE_SIGN_PASSWORD=your_password_here