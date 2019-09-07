open ReasonDateFns;

[@bs.module "../../../../src/localization/localization.js"]
external i18n: string => string = "translate";

[@bs.module "../../../../src/localization/localization.js"]
external i18nInterpolate: (string, Js.t('a)) => string = "translate";

module Date = {
  let getDateFnsLocale = () =>
    switch (Expo.Localization.locale) {
    | "ru"
    | "ru-BY"
    | "ru-MD"
    | "ru-UA" => ReasonDateFns.DateFns.Locales.ru
    | _ => ReasonDateFns.DateFns.Locales.enUS
    };

  let localizedFormat = DateFns.formatWithOptions(
    DateFns.formatOptions(~locale=getDateFnsLocale(), ())
  );

  let localizedFormatDistance =
    DateFns.formatDistanceWithOptions(
      DateFns.formatDistanceOptions(~locale=getDateFnsLocale(), ()),
    );
};