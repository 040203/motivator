open Styles;
open ReactNative;
open ReactNative.Style;

let styles =
  makeStyles(theme =>
    {
      "pageTitle":
        style(
          ~fontSize=36.,
          ~marginBottom=theme.spacing(2.),
          ~fontWeight=`bold,
          (),
        ),
      "container":
        style(
          ~flex=1.,
          ~padding=theme.spacing(1.),
          ~paddingTop=theme.spacing(4.),
          ~marginTop=theme.spacing(4.),
          ~alignItems=`center,
          ~backgroundColor="#F5FCFF",
          (),
        ),
      "tabs":
        style(
          ~width=pct(100.),
          ~marginTop=auto,
          ~borderTopWidth=1.,
          ~borderTopColor="#eff0f1",
          ~flexDirection=`row,
          ~justifyContent=`spaceAround,
          (),
        ),
    }
  );

type routes =
  | Transformation
  | Wieght
  | Statistics;

[@react.component]
let make = () => {
  let (route, navigate) = React.useState(_ => Wieght);
  let makeRouteNavigation = (route, _) => navigate(_ => route);
  let title =
    switch (route) {
    | Wieght => "Progress"
    | Transformation => "Transformation"
    | Statistics => "Statistics"
    };

  <SafeAreaView style=styles##container>
    <Text style=styles##pageTitle> {React.string(title)} </Text>
    <ScrollView>
      {switch (route) {
       | Wieght => <ProfileRe navigate />
       | Transformation => <ProfileRe navigate />
       | Statistics => <ProfileRe navigate />
       }}
    </ScrollView>
    <View style=styles##tabs>
      <IconButton
        selected={route === Transformation}
        name="dumbbell"
        onPress={Transformation->makeRouteNavigation}
      />
      <IconButton
        selected={route === Wieght}
        name="weight"
        onPress={Wieght->makeRouteNavigation}
      />
      <IconButton
        selected={route === Statistics}
        name="chart-bar"
        onPress={Statistics->makeRouteNavigation}
      />
    </View>
  </SafeAreaView>;
};