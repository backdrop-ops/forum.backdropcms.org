(function($, Drupal) {
  /**
   * Add new command for reading a message.
   */
  Drupal.ajax.prototype.commands.getVotes = function(ajax, response, status){
    // Place content in ajax-target div.
    var cid = response.cid;
    var uid = response.uid;
    var type = response.type;
    var score = response.score;
    $('#ajax-target').html(
      score
    );
  }
})(jQuery, Drupal);
