open Styles;
open ReactNative;
open ReactNative.Style;

let styles =
  makeStyles(theme =>
    {
      "pageContainer": style(~width=pct(100.), ~alignItems=`center, ()),
      "container":
        style(
          ~flex=1.,
          ~padding=theme.spacing(1.),
          ~alignItems=`center,
          ~backgroundColor="#F5FCFF",
          (),
        ),
      "tabs":
        style(
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
    <ScrollView style={style(~width=pct(100.), ())}>
      <View style=styles##pageContainer>
        <StyledText variant=LargeTitle value=title />
        {switch (route) {
         | Wieght => <WeightScreen />
         | Transformation => <WeightScreen />
         | Statistics => <WeightScreen />
         }}
      </View>
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