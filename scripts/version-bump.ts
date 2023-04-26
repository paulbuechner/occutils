import { Octokit } from "@octokit/rest";

import fs from "fs";

const octokit = new Octokit({
  auth: process.env.GH_TOKEN,
});

async function updateVcpkgFmtTemplateVersion(): Promise<void> {
  // Fetch the latest release of the repository
  const { data: release } = await octokit.rest.repos.getLatestRelease({
    owner: "paulbuechner",
    repo: "occutils",
  });

  // Remove the "v" in the release tag
  const releaseVersion = release.tag_name.replace("v", "");

  // Update CMakeLists.txt
  const cmakeFilePath = "CMakeLists.txt";
  const cmakeFileContent = fs.readFileSync(cmakeFilePath, "utf-8");
  const updatedCMakeContent = cmakeFileContent.replace(
    /set\(VCPKG_FMT_TEMPLATE_VERSION\s+\S+\)/,
    `set(VCPKG_FMT_TEMPLATE_VERSION ${releaseVersion})`
  );
  fs.writeFileSync(cmakeFilePath, updatedCMakeContent, "utf-8");

  // Update vcpkg.json
  const vcpkgJsonPath = "vcpkg.json";
  const vcpkgJsonContent = fs.readFileSync(vcpkgJsonPath, "utf-8");
  const vcpkgJson = JSON.parse(vcpkgJsonContent);
  vcpkgJson.version = releaseVersion;
  const updatedVcpkgJsonContent = JSON.stringify(vcpkgJson, null, 2);
  fs.writeFileSync(vcpkgJsonPath, updatedVcpkgJsonContent, "utf-8");
}

updateVcpkgFmtTemplateVersion()
  .then(() => console.log("VCPKG_FMT_TEMPLATE_VERSION and vcpkg.json updated"))
  .catch((error) => console.error("Failed to update versions", error));
