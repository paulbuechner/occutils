import getReleasePlan from "@changesets/get-release-plan";
import type { ComprehensiveRelease, NewChangeset } from "@changesets/types";

const startCase = (text: string) => {
  return text
    .split("-")
    .map((word) => {
      return word.charAt(0).toUpperCase() + word.slice(1);
    })
    .join(" ");
};

const padStart = (text: string, length: number, char: string) => {
  return text.padStart(length, char);
};

export const addonPkg = "storybook-addon-data-theme-switcher";

export function getCurrentDate() {
  const date = new Date();
  const day = padStart(date.getDate().toString(), 2, "0");
  const month = padStart((date.getMonth() + 1).toString(), 2, "0");
  const year = date.getFullYear();

  return `## ${day}-${month}-${year}`;
}

/* -----------------------------------------------------------------------------
 * Collect and sort the changesets
 * -----------------------------------------------------------------------------*/

type Options = {
  cwd: string;
  sinceRef?: string;
  exclude?: string[];
};

export async function getChangesetEntries(options: Options) {
  const { cwd, sinceRef, exclude = [addonPkg] } = options;

  const releasePlan = await getReleasePlan(cwd, sinceRef);

  const releases = releasePlan.releases
    .filter(
      (release) => release.name === addonPkg || release.changesets.length > 0
    )
    .filter((release) => !exclude.includes(release.name))
    .sort((a, b) => {
      if (a.name === addonPkg) {
        return -1;
      }
      if (b.name === addonPkg) {
        return 1;
      }
      return 0;
    });

  return {
    releases,
    summary: releases.map((release) =>
      getReleaseSummary(releasePlan.changesets, release)
    ),
  };
}

/* -----------------------------------------------------------------------------
 * Get the release summary for each release
 * -----------------------------------------------------------------------------*/

export function getReleaseSummary(
  changesets: NewChangeset[],
  release: ComprehensiveRelease
) {
  const formattedChangesets = release.changesets.map((changeset) => {
    const { summary } = changesets.find((cs) => cs.id === changeset) ?? {};
    return !summary || summary?.trim().startsWith("-")
      ? summary
      : `- ${summary} \n`;
  });

  const displayName = `\`${addonPkg}@${release.newVersion}\``;

  return {
    ...release,
    changesets: formattedChangesets,
    displayName: displayName.replace(/,\s*$/, ""),
  };
}
