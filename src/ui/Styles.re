open ReactNative.Style;

type palette = {
  primary: string,
  primaryLight: string,
  primaryDark: string,
  grey: string,
};

type themeType = {
  palette,
  spacing: float => size,
};

let theme = {
  palette: {
    primary: "#673ab7",
    primaryLight: "#7e57c2",
    primaryDark: "#512da8",
    grey: "#bdbdbd",
  },
  spacing: dpValue => dp(dpValue *. 8.),
};

let makeStyles = styleCreateFn =>
  ReactNative.StyleSheet.create(styleCreateFn(theme));

let shared =
  makeStyles(theme =>
    {
      "largeButton":
        style(~width=90.->pct, ~backgroundColor=theme.palette.primary, ()),
    }
  );