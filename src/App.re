open ReactNative;
open ReactNative.Style;

let styles =
  StyleSheet.create({
    "container":
      style(
        ~flex=1.,
        ~justifyContent=`center,
        ~alignItems=`center,
        ~backgroundColor="#F5FCFF",
        (),
      ),
    "instructions": style(~textAlign=`center, ~color="red", ()),
  });

[@react.component]
let app = () => {
  <AuthenticateInvariant>
    <View style=styles##container>
      <Text style=styles##instructions>
        {React.string("To get started, edit App.re")}
      </Text>
    </View>
  </AuthenticateInvariant>;
};