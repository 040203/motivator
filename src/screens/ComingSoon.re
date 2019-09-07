open ReactNative;
open ReactNative.Style;

let window = Dimensions.get(`window);

[@react.component]
let make = () => {
  <>
   <StyledText textAlign=`center size=22. i18n="Coding jumbo is working on" />
    <View
      style={style(
        ~position=`relative,
        ~width=window##width->dp,
        ~height=(window##height *. 0.5)->dp,
        (),
      )}>
      <Lottie
        autoPlay=true
        source={Packager.require(
          "../../../../assets/animations/codingJumbo.json",
        )}
      />
    </View>
    <StyledText textAlign=`center size=22. i18n="Keep calm and stay tuned" />
  </>;
};