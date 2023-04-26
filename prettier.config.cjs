/** @type {import("prettier").Config} */
module.exports = {
  plugins: [
    require.resolve("prettier-plugin-packagejson"),
  ],
  trailingComma: "es5",
  tabWidth: 2,
  semi: true,
  singleQuote: false,
  arrowParens: "always",
  useTabs: false,
  printWidth: 80,
  endOfLine: "lf",
};
