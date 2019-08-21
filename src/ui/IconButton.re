open ReactNative;
open ReactNative.Style;

let styles =
  Styles.makeStyles(theme =>
    {
      "container":
        style(~flex=1., ~alignItems=`center, ~padding=theme.spacing(1.), ()),
    }
  );

[@react.component]
let make = (~selected=false, ~onPress, ~name, ~size=26) => {
  let color =
    selected ? Styles.theme.palette.primary : Styles.theme.palette.grey;

  <TouchableWithoutFeedback onPress>
    <View style=styles##container>
      <Icons.FontAwesome5 name size color />
    </View>
  </TouchableWithoutFeedback>;
};